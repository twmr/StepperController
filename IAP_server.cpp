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
#include <boost/lexical_cast.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_io.hpp>
#include <boost/algorithm/string/predicate.hpp>

#include "IAP_server.hpp"
#include "IAPBoard.hpp"
#include "rs232.hpp"
#include "helper.hpp"

using namespace std;

// mutex m;
STD_TR1::shared_ptr<IAPBoard> board(static_cast<IAPBoard*>(0));//(nullptr);

const char* helpMessage =
{
    "HELP:\n"
    "available commands are:\n"
    "quit          : quit the client\n" //implemented in the client
    "close         : stop the current session\n"
    "serialtest    : test rs232 communication\n"
    "\n"
    "sleep N       : sleep N seconds\n" //implemented in the client
    "set axis N    : setaxis where N is the axisnumber [0-2]\n"
    "set/unset jog : enable or disable jog mode\n"
    "mr a,b,c      : move relative a,b,c are real numbers\n"
    "ma a,b,c      : move absolute a,b,c are real numbers\n"
    //"printconvconsts       : print the conversion constants\n"
    //"set convconsts a,b,c  : setthe conversion constants\n"
    "set zero      : set the current position to zero\n"
    "pp            : print positions of all motors in user defined units\n"
    "pbp           : print bare positions of all motors (stepperboard units)"

};

/* in this file is the state of the last run stored */
std::string lastrunfilename("pm381.laststate");

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
    else if(! data.compare("serialtest")) {
        // fixme lock this
        board->test(message.msg);
    }
    else if(! data.compare("close")) {
        prepare_tcp_message("stopping current session");
        //FIXME
        //delete new_session;
    }
    else if(! data.compare("pbp")) {
        //print bare positions
        BarePosition bp;
        board->get_cur_position(bp);
        sprintf(message.msg, " axis1: %d\n axis2: %d\n axis3: %d\n", bp.get_x(), bp.get_y(), bp.get_theta());
    }
    else if(! data.compare("pp")) {
        //print positions
        Position p;
        board->get_cur_position(p);
        sprintf(message.msg, " axis1: %f\n axis2: %f\n axis3: %f\n", p.get_x(), p.get_y(), p.get_theta());
    }
    else {
        /* all other commads */

        //boost::regex e("^(setaxisnum )|(sa)([0-2])$");
        boost::regex e("^(sa|set axis )([[:digit:]])$");
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
                prepare_tcp_err_message(board->get_err_string(static_cast<pm381_err_t>(-ret)));
            else
                prepare_tcp_message("TODO return message");
            return;
        }

        //check set/unset commands
        if (boost::starts_with(data, "set ")) {
            std::string setvar = data.substr(4);
            std::cout << "setting " << setvar << std::endl;

            if(! setvar.compare("jog")) {
                //enable jog mode
                ret = board->send_command("1AR", message.msg);
                if(ret < 0)
                    prepare_tcp_err_message(board->get_err_string(static_cast<pm381_err_t>(-ret)));
            }
            else if(! setvar.compare("zero")) {
                // set current position to zero - this doesn't move
                // the stepper, only updates position inside the
                // Memory of the Stepper Controller Hardware
                board->SetZero();
                prepare_tcp_message("TODO setzero return message");
            }
            else
                prepare_tcp_message("invalid set command (check syntax)");
            return;
        }
        if (boost::starts_with(data, "unset ")) {
            std::string unsetvar = data.substr(6);
            std::cout << "unsetting " << unsetvar << std::endl;

            if(! unsetvar.compare("jog")) {
                //disable jog mode
                ret = board->send_command("1IR", message.msg);
                if(ret < 0)
                    prepare_tcp_err_message(board->get_err_string(static_cast<pm381_err_t>(-ret)));
            }
            else
                prepare_tcp_message("invalid unset command (check syntax)");
            return;
        }

        // movement commands
        if (boost::starts_with(data, "ma ")) { // move absolute
            std::string tmp = data.substr(3);
            std::vector<Position::type> posvec(3);

            if(!helper::parse_triple<Position::type>(tmp, posvec)) {
                board->move_to(Position(Position(posvec)));
                prepare_tcp_message("Success");
            }
            else
                prepare_tcp_message("parse_triple_failure");

            return;
        }
        else if (boost::starts_with(data, "mr ")) { // move relative
            std::string tmp = data.substr(3);
            std::vector<Position::type> posvec(3);

            if(!helper::parse_triple<Position::type>(tmp, posvec)) {
                board->move_rel(Position(Position(posvec)));
                prepare_tcp_message("Success");
            }
            else
                prepare_tcp_message("parse_triple_failure");

            return;
        }


        boost::regex e2("^[[:digit:]][[:alpha:]]+-?[[:digit:]]*$");

        if(boost::regex_match(data, what, e2, boost::match_extra)) {
            ret = board->send_command(data,message.msg);
            if(ret < 0)
                prepare_tcp_err_message(board->get_err_string(static_cast<pm381_err_t>(-ret)));
            return;
        }
        else
            prepare_tcp_message("invalid command (check syntax)");
    }
}

void catch_int(int sig)
{
    if(!board->is_connected())
        exit(1);

    BarePosition bp;
    board->get_cur_position(bp);
    std::cout << "writing current state to file" << lastrunfilename << std::endl;
    std::ofstream f(lastrunfilename.c_str(), ios_base::out | ios_base::trunc);
    f << bp.get_x() << ", "<<  bp.get_y() << ", "<< bp.get_theta() << std::endl;
    f.close();
    exit(1);
}



int main(int argc, char** argv)
{
    int ret = 0;
    const std::string serialconfigfilename("rsconf");
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

        //sets up axis,...
        board->connect();

        std::ifstream f(lastrunfilename.c_str(), ifstream::in);
        if (f) {
            char cstring[256];
            std::vector<BarePosition::type> posvec(3);
            std::cout << "reading position from "<< lastrunfilename << " file"
                      << std::endl;

            while (!f.eof())
            {
                f.getline(cstring, sizeof(cstring));
                if (cstring[0] != '#')
                {
                    string s(cstring);
                    std::cout << s << endl;
                    int ret =  helper::parse_triple(s,posvec);
                    if(ret < 0)
                    {
                        std::cerr << "ERROR: parse_triple returned " << ret << std::endl;
                    }


                    // BarePosition cbp;
                    // board->get_cur_position(cbp);
                    // std::cout << "current postion:" << std::endl;
                    // cbp.PrintPosition();

                    //FIXME move this send command stuff to the initAxes functino!
                    // and switch to XML files / boost::propertytree!!
                    BarePosition bp(posvec);
                    board->setaxisnum(0);
                    board->send_command_quiet("1SP" + boost::lexical_cast<std::string>(bp.get_x()));
                    board->setaxisnum(1);
                    board->send_command_quiet("1SP" + boost::lexical_cast<std::string>(bp.get_y()));
                    board->setaxisnum(2);
                    board->send_command_quiet("1SP" + boost::lexical_cast<std::string>(bp.get_theta()));
                    board->setaxisnum(0);


                    std::cout << "bare positions from last run:"
                              << "\n\taxis 1: " << bp.get_x()
                              << "\n\taxis 2: " << bp.get_y()
                              << "\n\taxis 3: " << bp.get_theta() << std::endl;
                    std::cout << "move to it ....";
                    board->move_to(bp);
                    std::cout << " done" << std::endl;
                    break;
                }
            }
            f.close();
        }

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
