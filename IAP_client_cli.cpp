#include <fstream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
#include <boost/asio.hpp>

#include "global.hpp"
#include "IAP_server.hpp"


using boost::asio::ip::tcp;


int main(int argc, char* argv[])
{
    try
    {
        if( argc < 2 ){
            std::cout << "Usage: " << *argv << " server-hostname  [cmd-file]" << std::endl;
            return 1;
        }

        std::string serverhost = argv[1];
        std::string cmd;
        std::ifstream f;

        bool read_cmd_list = false;
        bool send_single_cmd = false;
        msg_t request, reply;

        if(argc==3) {

            f.open(argv[2], std::ifstream::in);
            f.close();
            if(f.fail()) {
                send_single_cmd = true;
            }else
            {
                f.clear(std::ios::failbit);
                f.open(argv[2], std::ifstream::in);

                //do we still need to check if opening file succeeded?
                if(f.is_open() != true) {
                    std::cerr << "couldn't open " <<argv[2] << std::endl;
                    return 1;
                }
                read_cmd_list = true;
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


        using namespace std; // For strlen.

        while(true) {
            std::cout << "#> ";

            if(read_cmd_list) {
                f >> cmd;
                if(f.eof()) break;
                std::cout << cmd << std::endl;
            } else
            {
                std::cin.getline(request.msg, MSGSIZE);
                cmd=request.msg;
            }

            if(!cmd.compare("quit") || !cmd.compare("q")) {
                /* disconnect from tcp/ip server */
                break;
            }

            if(!cmd.length()) // empty string
                continue;

            if(read_cmd_list)
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
