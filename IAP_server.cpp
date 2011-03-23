/*
 * IAP_server.cpp
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

#include <cstdlib>
#include <iostream>
#include <signal.h> //FIXME not available for non unix systems
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/regex.hpp>

#include "IAP_server.hpp"
#include "IAPBoard.hpp"

using namespace std;

// mutex m;
STD_TR1::shared_ptr<IAPBoard> board(static_cast<IAPBoard*>(0));//(nullptr);

const char* helpMessage =
{
    "HELP:\n"
    "available commands are:\n"
    "quit       : quit the client\n"
    "close      : stop the current session\n"
    "connect    : start up stepperboard\n"
    "disconnect : shut down stepperboard\n"
    "serialtest : test rs232 communication\n"
    "\n"
    "saN        : setaxis where N is the axisnumber [0-2]\n"
    "enjog | disjog  enable or disable jog mode\n"
    "pp         : print positions of all motors"

};

/* in this file is the state of the last run stored */
std::string lastrunfilename("pm301.laststate");

using boost::asio::ip::tcp;

class server
{
public:
    server(boost::asio::io_service& io_service, short port)
        : io_service_(io_service),
          acceptor_(io_service, tcp::endpoint(tcp::v4(), port))
        {
            session* new_session = new session(io_service_);
            acceptor_.async_accept(new_session->socket(),
                                   boost::bind(&server::handle_accept, this, new_session,
                                               boost::asio::placeholders::error));
        }

    void handle_accept(session* new_session,
                       const boost::system::error_code& error)
        {
            if (!error)
            {
                std::cout << "new connection accepted" << std::endl;
                new_session->start();
                new_session = new session(io_service_);
                acceptor_.async_accept(new_session->socket(),
                                       boost::bind(&server::handle_accept, this, new_session,
                                                   boost::asio::placeholders::error));
            }
            else
            {
                delete new_session;
            }
        }

private:
    boost::asio::io_service& io_service_;
    tcp::acceptor acceptor_;
};


void session::process_msg(msg_t& message)
{
    //cout << "Got Command: " << message.msg << endl;
    string data(message.msg);
    message.type = MSG_REPLY;
    int ret = 0;

    if(! data.compare("help")) {
        prepare_tcp_message(helpMessage);
    }
    else if(! data.compare("connect")) {
        prepare_tcp_message("connecting...");
        // fixme lock this
        board->connect();
    }
    else if(! data.compare("disconnect")) {
        // fixme lock this
        board->disconnect();
        prepare_tcp_message("disconnected");
    }
    else if(! data.compare("serialtest")) {
        // fixme lock this
        board->test(message.msg);
    }
    else if(! data.compare("close")) {
        prepare_tcp_message("stopping current session");
        //FIXME
        //delete new_session;
    }
    else if(! data.compare("enjog")) {
        //enable jog mode
        ret = board->send_command("1AR", message.msg);
        if(ret < 0) {
            prepare_tcp_err_message(board->get_err_string(static_cast<pm301_err_t>(-ret)));
        }
    }
    else if(! data.compare("disjog")) {
        //disable jog mode
        ret = board->send_command("1IR", message.msg);
        if(ret < 0) {
            prepare_tcp_err_message(board->get_err_string(static_cast<pm301_err_t>(-ret)));
        }
    }
    else if(! data.compare("pp")) {
        //print positions
        int pos[board->get_nr_axis()];
        int oldaxisnum = board->getaxisnum();

        for (int i = 0; i < board->get_nr_axis(); ++i)
        {
            ret = board->setaxisnum(i);
            if(ret < 0) {
                prepare_tcp_err_message(board->get_err_string(static_cast<pm301_err_t>(-ret)));
                return;
            }

            pos[i] = board->send_getint_command("1OC");
        }

        ret = board->setaxisnum(oldaxisnum);
        if(ret < 0) {
            prepare_tcp_err_message(board->get_err_string(static_cast<pm301_err_t>(-ret)));
            return;
        }
        sprintf(message.msg, " axis1: %d\n axis2: %d\n axis3: %d", pos[0], pos[1], pos[2]);
    }
    else {
        /* all other commads */

        //boost::regex e("^(setaxisnum )|(sa)([0-2])$");
        boost::regex e("^(sa)([[:digit:]])$");
        boost::smatch what;
        if(boost::regex_match(data, what, e, boost::match_extra))
        {
            unsigned int axisnum;

            if(what.size() < 3)
                std::cerr << "not a valid regexp match (whole match " << what[0] <<
                    " )" << std::endl;

            axisnum = atoi(std::string(what[2]).c_str());

            //std::cout << "axisnum from regex: " << axisnum << std::endl;
            ret = board->setaxisnum(axisnum);
            if(ret < 0)
                prepare_tcp_err_message(board->get_err_string(static_cast<pm301_err_t>(-ret)));
            else
                prepare_tcp_message("TODO return message");
            return;
        }

        boost::regex e2("^[[:digit:]][[:alpha:]]+-?[[:digit:]]*$");

        if(boost::regex_match(data, what, e2, boost::match_extra)) {
            ret = board->send_command(data,message.msg);
            if(ret < 0)
                prepare_tcp_err_message(board->get_err_string(static_cast<pm301_err_t>(-ret)));
            return;
        }
        else
            prepare_tcp_message("invalid command (check syntax)");
    }
}

void catch_int(int sig)
{
    std::cout << "writing current state to file" << std::endl;
    std::ofstream f(lastrunfilename.c_str(), ios_base::out | ios_base::trunc);

    f << 12 << " "<<  3 << " "<< 4 << std::endl;
    f.close();
    exit(1);
}



int main(int argc, char** argv)
{
    int ret = 0;
    std::string serialconfigfilename("rsconf");
    std::string axisconfigfilename("axis.cfg");

    signal(SIGINT, catch_int);

    try
    {
        if(argc > 1)
            axisconfigfilename = argv[1];

        RS232config rsconfig(serialconfigfilename);
        IAPconfig axisconfig(axisconfigfilename);

        cout << "SERVER: init IAPBoard" << endl;
        board = STD_TR1::shared_ptr<IAPBoard>(new IAPBoard(rsconfig,axisconfig));

        std::ifstream f(lastrunfilename.c_str());
        char cstring[256];
        int pos[board->get_nr_axis()];

        if( f.is_open() != true) {
            std::cerr << "couldn't open "<< lastrunfilename << " file"
                      << std::endl;
            abort();
        }

        while (!f.eof())
        {
            f.getline(cstring, sizeof(cstring));
            if (cstring[0] != '#')
            {
                stringstream ss(cstring);
                ss >> pos[0] >> pos[1] >> pos[2];
                break;
            }
        }
        f.close();


        std::cout << "positions from last run:\n\taxis 1: "
                  << pos[0] << "\n\taxis 2: " << pos[1]
                  << "\n\taxis 3: " << pos[2] << std::endl;

        board->send_command_quiet("1IR"); // disable jog mode
        board->initAxis(); // set all axis speed etc.
        board->send_command_quiet("1WP0000"); // set axis 0


        boost::asio::io_service io_service;

        server s(io_service, IAPServer::port);

        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    catch_int(0);
    return ret;
}
