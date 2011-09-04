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
#include <fstream>
#include <signal.h> //FIXME not available for non unix systems
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_io.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "IAP_server.hpp"
#include "IAPBoard.hpp"
#include "helper.hpp"

using namespace std;

// mutex m;
STD_TR1::shared_ptr<IAPBoard> board(static_cast<IAPBoard*>(0));//(nullptr);

namespace IAPServer
{
    const char* helpMessage =
    {
        "HELP:\n"
        "available commands are:\n"
        "quit          : quit the client\n" //implemented in the client
        "close         : stop the current session\n"
        "serialtest    : test rs232 communication\n"
        "\n"
        "sleep N       : sleep N seconds\n" //implemented in the client
        "set axis N    : setaxis where N is the axisnumber [1-3]\n"
        "set/unset jog : enable or disable jog mode\n"
        "mr a,b,c      : move relative a,b,c are real numbers\n"
        "ma a,b,c      : move absolute a,b,c are real numbers\n"
        //"printconvconsts       : print the conversion constants\n"
        //"set convconsts a,b,c  : setthe conversion constants\n"
        "set zero      : set the current position to zero\n"
        "pp            : print positions of all motors in user defined units\n"
        "pbp           : print bare positions of all motors (stepperboard units)"

    };

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
                    cout << "new connection accepted" << endl;
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
#ifdef SERIAL_DEBUG
        else if(! data.compare("ERRORMSG_TEST")) {
            int ret = board->send_command("ERRORMSG_TEST", message.msg);
            if(ret < 0)
                prepare_tcp_err_message(
                    board->get_err_string(static_cast<pm381_err_t>(-ret), message.msg));
            else
                prepare_tcp_success_message();
        }
#endif
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
            BarePosition bp = board->createBarePosition();
            board->get_cur_position(bp);
            map<size_t,string>& id_string_map = board->get_coord_map();
            bp.GetPositionString(message.msg,id_string_map);
        }
        else if(! data.compare("pp")) {
            //print positions
            Position p = board->createPosition();
            board->get_cur_position(p);
            map<size_t,string>& id_string_map = board->get_coord_map();
            map<size_t,string>& id_unit_map = board->get_unit_map();
            p.GetPositionString(message.msg, id_string_map, id_unit_map);
        }
        else if (! data.compare("ga")) {
            //prints all registered axes + units + ids (used inside the GUI)
            map<size_t,string>& id_string_map = board->get_coord_map();
            map<size_t,string>& id_unit_map = board->get_unit_map();
            ostringstream os;

            for(map<size_t, string>::const_iterator it = id_string_map.begin();
                        it != id_string_map.end(); ++it){
                os << it->first << ":" << it->second << ":"
                   << id_unit_map[it->first] << std::endl;
            }
            prepare_tcp_message(os.str());
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
                    cerr << "not a valid regexp match (whole match " << what[0] <<
                        " )" << endl;

                axisnum = atoi(string(what[2]).c_str());

                //cout << "axisnum from regex: " << axisnum << endl;
                ret = board->setaxisnum(axisnum);
                if(ret < 0){
                    cout << " dummy " << ret << endl;
                    cout << board->get_err_string(static_cast<pm381_err_t>(-ret)) << endl;
                    prepare_tcp_err_message(board->get_err_string(static_cast<pm381_err_t>(-ret)));
                }

                else
                    prepare_tcp_success_message();
                return;
            }

            //check set/unset commands
            if (boost::starts_with(data, "set ")) {
                string setvar = data.substr(4);
                cout << "setting " << setvar << endl;

                if(! setvar.compare("jog")) {
                    //enable jog mode
                    ret = board->send_command("1AR", message.msg);
                    if(ret < 0)
                        prepare_tcp_err_message(
                            board->get_err_string(static_cast<pm381_err_t>(-ret), message.msg));
                    else
                        prepare_tcp_success_message();
                }
                else if(! setvar.compare("zero")) {
                    // set current position to zero - this doesn't move
                    // the stepper, only updates position inside the
                    // Memory of the Stepper Controller Hardware
                    board->SetZero();
                    prepare_tcp_message("TODO setzero return message");
                }
                else
                    prepare_tcp_err_message("invalid set command (check syntax)");
                return;
            }
            if (boost::starts_with(data, "unset ")) {
                string unsetvar = data.substr(6);
                cout << "unsetting " << unsetvar << endl;

                if(! unsetvar.compare("jog")) {
                    //disable jog mode
                    ret = board->send_command("1IR", message.msg);
                    if(ret < 0)
                        prepare_tcp_err_message(
                            board->get_err_string(static_cast<pm381_err_t>(-ret), message.msg));
                    else
                        prepare_tcp_success_message();
                }
                else
                    prepare_tcp_err_message("invalid unset command (check syntax)");
                return;
            }

            // movement commands (aboslute and relative)
            if (boost::starts_with(data, "ma ") || boost::starts_with(data, "mr ")) {
                string tmp = data.substr(3);
                map<string, Position::type> posmap;

                if(!helper::parse_multiplett<Position::type>(tmp, posmap)) {
                    Position mypos;
                    map<string,size_t>& string_id_map = board->get_inv_coord_map();

                    for(map<string, Position::type>::const_iterator it = posmap.begin();
                        it != posmap.end(); ++it){

                        //check that the entered axis descriptor is valid
                        if(string_id_map.find(it->first) == string_id_map.end()) {
                            prepare_tcp_err_message(
                                "entered position has an invalid axis descriptor");
                            return;
                        }

                        mypos.SetCoordinate(string_id_map[(*it).first], (*it).second);
                    }

                    if(mypos.size() == 0) {
                        prepare_tcp_err_message("entered position is not valid");
                        return;
                    }

                    cout << "requested new position: ";
                    for(Position::coord_type::const_iterator it = mypos.begin(); it != mypos.end(); ++it)
                        cout << it->first << ": " << it->second << " ";
                    cout << endl;

                    int ret;
                    if(data[1] == 'a') // absolute move
                        ret = board->move_to(mypos);
                    else // data[1] = 'r'  -> relative move
                        ret = board->move_rel(mypos);

                    if(ret < 0)
                        // todo use better error message
                        prepare_tcp_err_message("Controller Error");
                    else
                        prepare_tcp_success_message();
                }
                else
                    prepare_tcp_err_message("parse_multiplett_failure");

                return;
            }


            boost::regex e2("^[[:digit:]][[:alpha:]]+-?[[:digit:]]*$");

            if(boost::regex_match(data, what, e2, boost::match_extra)) {
                ret = board->send_command(data,message.msg);
                if(ret < 0)
                    prepare_tcp_err_message(
                        board->get_err_string(static_cast<pm381_err_t>(-ret), message.msg));
                return;
            }
            else
                prepare_tcp_err_message("invalid command (check syntax)");
        }
    }
}


void catch_int(int sig)
{
    if(!board->is_connected())
        exit(1);

    BarePosition bp;
    board->get_cur_position(bp);

    cout << "signal_handler:" << endl;
    for(BarePosition::coord_type::const_iterator
            it = bp.begin(); it != bp.end(); ++it) {
        cout << "\tstoring current pos of axis with id " << it->first
             << " in xml file" << endl;
        board->getConfig().setAxisElement(it->first, "Position", bp.GetCoordinate(it->first));
    }

    (board->getConfig()).writeconfig();
    _exit(1);
}



int main(int argc, char** argv)
{
    string configfilename("parameters.xml");

    // Read command line arguments
    try {
        po::options_description desc("Allowed options");
        desc.add_options()
            ("help,h", "produce help message")
            ("parameters,f", po::value(&configfilename),
             "xml file containing the parameters for the steppermotor hardware");

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("help")) {
            cout << desc << "\n";
            return 1;
        }
    }
    catch(exception& e) {
        cerr << "error: " << e.what() << "\n";
        return 1;
    }

    {
        ifstream is(configfilename);
        if(!is) {
            cerr << "can't open " << configfilename << endl;
            return -1;
        }
    }

    cout << "SERVER: parsing MOVES xml config file \"" << configfilename << "\" ... ";

    static IAPconfig config(configfilename);

    cout << "done" << endl;

    try
    {
        cout << "SERVER: init IAPBoard" << endl;
        board = STD_TR1::shared_ptr<IAPBoard>(new IAPBoard(config));
        cout << "SERVER: init IAPBoard done" << endl;

        signal(SIGINT, catch_int);

        board->connect();

        const unsigned short port = config.get<unsigned short>("config.server.port");
        cout << "SERVER: starting network server on port " << port << endl;
        boost::asio::io_service io_service;
        IAPServer::server s(io_service, port);

        io_service.run();
    }
    catch (exception& e) {
        cerr << "Exception: " << e.what() << endl;
    }

    catch_int(0);
    return 0;
}
