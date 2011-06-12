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
#include <boost/asio.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>

#include "global.hpp"
#include "IAP_server.hpp"


using boost::asio::ip::tcp;


int main(int argc, char* argv[])
{
    try
    {
        if( argc < 2 ){
            std::cout << "Usage: " << *argv << " server-hostname [batch-file]"
                      << std::endl;
            return 1;
        }

        std::string serverhost = argv[1];
        std::string cmd;
        std::ifstream f;

        bool batch_mode = false;
        bool send_single_cmd = false;
        msg_t request, reply;

        if(argc==3) {

            f.open(argv[2], std::ifstream::in);
            f.close();
            if(f.fail()) {
                send_single_cmd = true;
            }
            else {
                f.clear(std::ios::failbit);
                f.open(argv[2], std::ifstream::in);

                //do we still need to check if opening file succeeded?
                if(f.is_open() != true) {
                    std::cerr << "couldn't open " <<argv[2] << std::endl;
                    return 1;
                }
                batch_mode = true;
            }
        }

        boost::asio::io_service io_service;

        tcp::resolver resolver(io_service);
        tcp::resolver::query query(tcp::v4(), serverhost, "15000");
        tcp::resolver::iterator iterator = resolver.resolve(query);

        tcp::socket s(io_service);
        s.connect(*iterator);

        if(send_single_cmd) {
            strcpy(request.msg, argv[2]);
            request.type = MSG_REQUEST;
            boost::asio::write(s, boost::asio::buffer(reinterpret_cast<char*>(&request), msglen));
            boost::asio::read(s, boost::asio::buffer(reinterpret_cast<char*>(&reply), msglen));
            std::cout << reply.msg << std::endl;
            return 0;
        }

        while(true) {
            std::cout << "#> ";

            if(batch_mode) {
                f >> cmd;
                if(f.eof()) break;
                std::cout << cmd << std::endl;
            } else
            {
                // interactive mode
                std::cin.getline(request.msg, MSGSIZE);
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
                    std::cout << "sleeping for " << sleep_value << " seconds" << std::endl;
                    sleep(sleep_value);
                }
            } catch (std::bad_cast) {
                // bad parameter
                std::cerr << "bad sleep parameter" << std::endl;
            }

            if(batch_mode)
                strcpy(request.msg, cmd.c_str());


            request.type = MSG_REQUEST;
            boost::asio::write(s, boost::asio::buffer(reinterpret_cast<char*>(&request), msglen));
            boost::asio::read(s, boost::asio::buffer(reinterpret_cast<char*>(&reply), msglen));

            std::cout << reply.msg << std::endl;
        }

        f.close();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
