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
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/tokenizer.hpp>
#include <boost/tuple/tuple.hpp>
namespace po = boost::program_options;
namespace tp = boost::tuples;

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
    // std::cout << " - send ret: " << send_retval;

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

    // std::cout <<  " - received: len=" << strlen(buffer)
    //           << " buf: " << buffer;
    //           << std::endl;
           
    //stip last 3 chars from the Egun return message
    // last 3 chars are: Ascii 13, 10, 13
    buffer[strlen(buffer)-3] ='\0';

    // for(size_t i=0; i < strlen(buffer); ++i) {
    //     cout << i << ":\t" << static_cast<int>(buffer[i]);
    //     if(buffer[i] > 31 && buffer[i] != 127)
    //         cout << " " << buffer[i] << endl;
    //     else
    //         cout << endl;
    // }

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

    const vector<string> controls = { "Energy", "Source", "Grid", "fristAnode",
                                      "Focus", "n/a", "Xdeflection", "Ydeflection" };
    const vector<unsigned int> convfacs = { 10, 1000, 100, 10,
                                            10, 0, 100, 100 };
    const vector<tp::tuple<double, double, double, double> > defaultparams =
        { { 10.0, 0.12, 0.0, 13.9 },
          { 20, -0.36, 0.0,9.8},
          { 30, -0.74, 0.0,9.1},
          { 40, -0.90, 0.0,9.2},
          { 50, -1.0, 0.0,10},
          {100, -3, 0, 15},
          {200, -7, 0, 36},
          {300, -11, 0, 52},
          {400, -12, -1, 69},
          {500, -13, -5, 98},
          {600, -15, -1, 136.6},
          {700, -14, -3, 192},
          {800, -15, -2, 292},
          {900, -15, -3, 309},
          {1000, -15,-1, 339},
          {1100, -22,-4, 357},
          {1200, -26,-2, 390},
          {1300, -27,-5, 400},
          {1400, -22,-7, 441},
          {1500, -24,-10, 468},
          {1600, -27,-10, 499}
    };


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
        while(true) {
            if(batch_mode) {
                if(!batchcmds.size())
                    break;
                cout << "\033[0;31m$> \033[0m";
                cmd = batchcmds.front();
                batchcmds.erase(batchcmds.begin());
                cout << cmd << endl;
            } else
            {
                // interactive mode
                if(!send_single_cmd)
                    cmd = readline("\033[0;31m$> \033[0m");

                if(!cmd.compare("quit") || !cmd.compare("q")) {
                    /* disconnect from tcp/ip server */
                    break;
                }

                if(cmd.length())
                    add_history(cmd.c_str());
            }

            if(!cmd.length()) // empty string
                continue;

            bool found=false;
            bool load_defaults=false;
            double eval=0.0; // Energy value
            //maybe use find_if .....
            // for(std::vector<string>::const_iterator it = controls.begin(); it != controls.end(); ++it){

            // std::vector<string>::const_iterator fd = find(controls.begin(), controls.end(), cmd);
            // if(fd != controls.end())


            for(size_t idx = 0; idx < controls.size(); ++idx) {
                if(!boost::starts_with(cmd, controls[idx]))
                    continue;

                found = true;
                //check whether we want to set or get the value
                size_t pos = cmd.find_first_of('=');
                if(pos != string::npos) {
                    boost::replace_all(cmd, "\n", "");
                    // cout<< "|" << cmd << "|" << cmd.substr(pos+1) << "|" << endl;
                    double setval = boost::lexical_cast<double>(cmd.substr(pos+1));
                    cmd = "po:" + boost::lexical_cast<string>(idx) + ",";
                    cmd += boost::lexical_cast<string>(static_cast<int>(setval*convfacs[idx]));

                    if(idx==0 && (setval == floor(setval))) { // Energy command
                        load_defaults = true;
                        eval = setval;
                    }
                }
                else {
                    cmd = "go:" + boost::lexical_cast<string>(idx);
                }

                cout << "Command to send: "<< cmd << endl;
                string rpl = send_lowlevel(serial_interface, cmd);

                if(load_defaults) {
                    for(vector<tp::tuple<double,double,double,double> >::const_iterator it = defaultparams.begin();
                        it != defaultparams.end(); ++it){
                        if(it->get<0>() == eval)
                        {
                            cout << "Load default values for " << it->get<0>()
                                 << "eV" << endl;
                            //x-deflection
                            cmd = "po:6," + boost::lexical_cast<string>(
                                static_cast<int>(it->get<1>()*convfacs[6]));
                            cout << "Command to send: "<< cmd << endl;
                            send_lowlevel(serial_interface, cmd);
                            //y-deflection
                            cmd = "po:7," + boost::lexical_cast<string>(
                                static_cast<int>(it->get<2>()*convfacs[7]));
                            cout << "Command to send: "<< cmd << endl;
                            send_lowlevel(serial_interface, cmd);
                            //Vfocus
                            cmd = "po:4," + boost::lexical_cast<string>(
                                static_cast<int>(it->get<3>()*convfacs[4]));
                            cout << "Command to send: "<< cmd << endl;
                            send_lowlevel(serial_interface, cmd);
                            break;
                        }
                    }
                }

                pos = rpl.find_first_of(',');
                if(pos != string::npos) {
                    int curval = boost::lexical_cast<int>(rpl.substr(pos+1));
                    cout << "value: " << 1.0*curval/convfacs[idx] << endl;
                }
                else
                    cout << "Error:" << rpl << endl;
                break;
            }


            // if(reply.type == MSG_ERROR)
            //      cout << "\033[0;31mError: " << reply.msg << "\033[0m" << endl;
            // else if(reply.type == MSG_SUCCESS)
            //     cout << "\033[0;32m" << "OK" << "\033[0m" << endl;
            // else
            // cout << send_lowlevel(serial_interface, cmd) << endl;
            if(!found) {
                string rpl = send_lowlevel(serial_interface, cmd);
                cout << rpl << endl;
            }

            if(send_single_cmd)
                break;

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
