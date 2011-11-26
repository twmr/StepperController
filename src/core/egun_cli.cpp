/*
 * egun_cli.cpp
 *
 * Copyright (c) 2010-2011 Thomas Hisch <thomas at opentech dot at>.
 *
 * This file is part of StepperControl.
 *
 * StepperControl is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * StepperControl is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with StepperControl.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <fstream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
#include <vector>

#include <boost/asio.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/tokenizer.hpp>
namespace po = boost::program_options;

#include <readline/readline.h>
#include <readline/history.h>

#include <signal.h> //FIXME not available for non unix systems

#include "global.hpp"
#include "rs232.hpp"

using boost::asio::ip::tcp;
using namespace std;

class egunconfig {
public:
    egunconfig(const std::string & configfile) {
        params_.clear();
        read_xml(configfile, params_);
    };

    ~egunconfig() { };

    const boost::property_tree::ptree& GetParameters(const std::string& group) const {
        std::string pN = "config." + group;
        return params_.get_child(pN);
    }

    const boost::property_tree::ptree& GetParameters() const {
        return params_.get_child("config");
    }

    template <class T>
    const T get(const std::string &s) const { return params_.get<T>(s); };

    template <class T>
    void set(const std::string &s, T value) const { return params_.put(s, value); };
private:
    boost::property_tree::ptree params_;
    const std::string filename;
};



string send_lowlevel(STD_TR1::shared_ptr< RS232 >& sinf, string sendcmd)
{
    bool readflag = false;
    int len = 0;
    unsigned int idx = 0;
    size_t send_retval;
    static char buffer[1024];

    sendcmd += sinf->get_eos();

    /* write() is assumed to work in one run, as the commands sent
       to the serial port have a really small size */
    send_retval = sinf->send(sendcmd.c_str(), sendcmd.length());
    std::cout << " - send ret: " << send_retval;

    while(len <= 0 || readflag) {
        usleep(sinf->get_timeout());

        //std::cout << "db1" << std::endl;
        len = sinf->receive(&buffer[idx], sizeof(buffer)-idx);
        //std::cout << "db2 len " << len << std::endl;
        if(len <= 0) {
            //std::cout << "db3" << std::endl;
            if(readflag) break;
        }
        else {
            //std::cout << "db4" << std::endl;
            readflag = true;
            idx += len;
        }
    }

#ifdef SERIAL_DEBUG // if defined run without hardware
    if(cmd == "ERRORMSG_TEST")
        sprintf(buffer,"01:! this is a sample error msg\r\n");
#endif

     std::cout <<  " - received: len=" << strlen(buffer)
               << std::endl;
               // << " buf: " << buffer;

    return std::string(buffer);
}

void catch_int(int sig)
{
    write_history(".egun.hist");
    exit(0);
}

int main(int argc, char* argv[])
{
    string batch;
    string cmd;
    bool batch_mode = false;
    vector<string> batchcmds;
    bool send_single_cmd = false;
    string configfile("egun.xml");

    // Read command line arguments
    try {
        po::options_description desc("Allowed options");
        desc.add_options()
            ("help,h", "produce help message")
            ("batchfile,b", po::value<string>(),
             "name of the batchfile to execute on the steppermotor hardware")
            ("cmd,c", po::value<string>(),
             "only send a single command to the sever, cmd needs to be put in parenthesis");

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("help")) {
            cout << desc << "\n";
            return 1;
        }
        if (vm.count("batchfile")) {
            //load file
            ifstream ifs(vm["batchfile"].as<string>().c_str());
            if (!ifs) {
                cout << "Could not open the batch file\n";
                return 1;
            }
            // Read the whole file into a string
            stringstream ss;
            ss << ifs.rdbuf();
            // Split the file content
            boost::char_separator<char> sep("\n\r");
            string batchfilecontents( ss.str() );
            boost::tokenizer<boost::char_separator<char> > tok(batchfilecontents, sep);
            copy(tok.begin(), tok.end(), back_inserter(batchcmds));
            batch_mode = true;
        }
        if (vm.count("cmd")) {
            send_single_cmd = true;
            cmd = vm["cmd"].as<string>();
        }
    }
    catch(exception& e) {
        cerr << "error: " << e.what() << "\n";
        return 1;
    }

    try
    {
        egunconfig config(configfile);
        STD_TR1::shared_ptr< RS232 > serial_interface(new sctl_ctb(config.GetParameters("rs232")));
        read_history(".egun.hist");
        signal(SIGINT, catch_int);

        serial_interface->open();
        if(send_single_cmd) {
            cout << "sending cmd \"" << cmd << "\" to server" << endl;
            return 0;
        }
        while(true) {
            if(batch_mode) {
                cout << "#> ";
                if(!batchcmds.size())
                    break;
                cmd = batchcmds.front();
                batchcmds.erase(batchcmds.begin());
                cout << cmd << endl;
            } else
            {
                // interactive mode

                cmd = readline("$>");


                if(!cmd.compare("quit") || !cmd.compare("q")) {
                    /* disconnect from tcp/ip server */
                    break;
                }

                if(cmd.length())
                    add_history(cmd.c_str());
            }

            if(!cmd.length()) // empty string
                continue;

            // if(reply.type == MSG_ERROR)
            //      cout << "\033[0;31mError: " << reply.msg << "\033[0m" << endl;
            // else if(reply.type == MSG_SUCCESS)
            //     cout << "\033[0;32m" << "OK" << "\033[0m" << endl;
            // else
            // cout << send_lowlevel(serial_interface, cmd) << endl;
            string rpl = send_lowlevel(serial_interface, cmd);
            cout << rpl << endl;
            // for(size_t i=0; i < rpl.length(); ++i) {
            //     cout << i << ":\t" << static_cast<int>(rpl[i]);
            //     if(rpl[i] > 31 && rpl[i] != 127)
            //         cout << " " << rpl[i] << endl;
            //     else
            //         cout << endl;
            // }


        }

    } catch (exception& e) {
        cerr << "Exception: " << e.what() << endl;
    }

    write_history(".egun.hist");
    return 0;
}
