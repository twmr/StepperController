/*
 * IAP_client_cli.cpp
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
#include <boost/tokenizer.hpp>
namespace po = boost::program_options;

#include "global.hpp"
#include "IAP_server.hpp"


using boost::asio::ip::tcp;
using namespace std;

int main(int argc, char* argv[])
{

    string serverhostname("localhost");
    string serverport("16000"); //string is ok here
    string batch;
    string cmd;
    bool batch_mode = false;
    vector<string> batchcmds;
    bool send_single_cmd = false;

    // Read command line arguments
    try {
        po::options_description desc("Allowed options");
        desc.add_options()
            ("help,h", "produce help message")
            ("serverhostname,s", po::value(&serverhostname),
             "hostname of the server")
            ("serverport,p", po::value(&serverport),
             "portname of the server")
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
        msg_t request, reply;

        boost::asio::io_service io_service;

        tcp::resolver resolver(io_service);
        tcp::resolver::query query(tcp::v4(), serverhostname, serverport);
        tcp::resolver::iterator iterator = resolver.resolve(query);

        tcp::socket s(io_service);
        s.connect(*iterator);

        strcpy(request.msg, "state");
        request.type = MSG_REQUEST;
        boost::asio::write(s, boost::asio::buffer(reinterpret_cast<char*>(&request), msglen));
        boost::asio::read(s, boost::asio::buffer(reinterpret_cast<char*>(&reply), msglen));
        std::string rpl = reply.msg;
        // cout << "state is " << reply.msg << endl;
        if(boost::starts_with(rpl,"initialize")) {
            cerr << "client console is disabled during initialization process" << endl;
            return -1;
        }

        if(send_single_cmd) {
            cout << "sending cmd \"" << cmd << "\" to server" << endl;
            strcpy(request.msg, cmd.c_str());
            request.type = MSG_REQUEST;
            boost::asio::write(s, boost::asio::buffer(reinterpret_cast<char*>(&request), msglen));
            boost::asio::read(s, boost::asio::buffer(reinterpret_cast<char*>(&reply), msglen));
            cout << reply.msg << endl;
            return 0;
        }

        while(true) {
            cout << "#> ";

            if(batch_mode) {
                if(!batchcmds.size())
                    break;
                cmd = batchcmds.front();
                batchcmds.erase(batchcmds.begin());
                cout << cmd << endl;
            } else
            {
                // interactive mode
                cin.getline(request.msg, MSGSIZE);
                cmd=request.msg;

                if(!cmd.compare("quit") || !cmd.compare("q")) {
                    /* disconnect from tcp/ip server */
                    break;
                }
            }

            if(!cmd.length()) // empty string
                continue;


            try {
                if (boost::starts_with(cmd, "sleep ")) {
                    int sleep_value = boost::lexical_cast<int>(cmd.substr(6));
                    cout << "sleeping for " << sleep_value << " seconds" << endl;
                    sleep(sleep_value);
                    continue;
                }
            } catch (bad_cast) {
                // bad parameter
                cerr << "bad sleep parameter" << endl;
            }

            if(batch_mode)
                strcpy(request.msg, cmd.c_str());


            request.type = MSG_REQUEST;
            boost::asio::write(s, boost::asio::buffer(reinterpret_cast<char*>(&request), msglen));
            boost::asio::read(s, boost::asio::buffer(reinterpret_cast<char*>(&reply), msglen));

            if(reply.type == MSG_ERROR)
                 cout << "\033[0;31mError: " << reply.msg << "\033[0m" << endl;
            else if(reply.type == MSG_SUCCESS)
                cout << "\033[0;32m" << "OK" << "\033[0m" << endl;
            else
                cout << reply.msg << endl;

        }

    } catch (exception& e) {
        cerr << "Exception: " << e.what() << endl;
    }

    return 0;
}
