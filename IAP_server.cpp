#include <cstdlib>
#include <iostream>
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

    if(! data.compare("help")) {
        async_send(helpMessage);
        return;
    }

    if(! data.compare("connect")) {
        async_send("connecting...");
        // fixme lock this
        board->connect(*this);
        return;
    }

    if(! data.compare("disconnect")) {
        // fixme lock this
        board->disconnect();
        async_send("disconnected");
        return;
    }

    if(! data.compare("serialtest")) {
        // fixme lock this
        board->test(*this);
        return;
    }

    if(! data.compare("close")) {
        async_send("stopping current session");
        //FIXME
        //delete new_session;
    }

    if(! data.compare("enjog")) {
        //enable jog mode
        board->send_command(*this, "1AR");
        return;
    }
    if(! data.compare("disjog")) {
        //disable jog mode
        board->send_command(*this, "1IR");
        return;
    }

    if(! data.compare("pp")) {
        //print positions
        int pos[board->get_nr_axis()];
        int oldaxisnum = board->getaxisnum();

        for (int i = 0; i < board->get_nr_axis(); ++i)
        {
            board->setaxisnum(*this,i,true);
            pos[i] = board->send_command_quiet("1OC");
        }

        board->setaxisnum(*this,oldaxisnum,true);
        sprintf(message.msg, " axis1: %d\n axis2: %d\n axis3: %d", pos[0], pos[1], pos[2]);
        return;
    }

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
        board->setaxisnum(*this,axisnum,false);
        return;
    }
    boost::regex e2("^[[:digit:]][[:alpha:]]+-?[[:digit:]]*$");

    if(boost::regex_match(data, what, e2, boost::match_extra))
        board->send_command(*this, data);
    else
        async_send("invalid command (check syntax)");

}

int main(int argc, char** argv)
{
    int ret = 0;
    std::string serialconfigfilename("rsconf");
    std::string axisconfigfilename("axis.cfg");
    //std::vector<boost::shared_ptr<boost::thread> > threads;

    try
    {
        if(argc > 1)
            axisconfigfilename = argv[1];

        RS232config rsconfig(serialconfigfilename);
        IAPconfig axisconfig(axisconfigfilename);

        cout << "SERVER: init IAPBoard" << endl;
        board = STD_TR1::shared_ptr<IAPBoard>(new IAPBoard(rsconfig,axisconfig));

        boost::asio::io_service io_service;

        using namespace std; // For atoi.
        server s(io_service, IAPServer::port);

        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }



    //try {
    // ServerSock server ( IAPServer::port );
    //     while ( true ) {
    //         ServerSock new_sock;
    //         server.accept ( new_sock );
    //         new_sock << "lulu";
    //         cout << "SERVER: client connected: starting new tcpip thread" << endl;
    //     }
    // boost::shared_ptr<boost::thread> thread(new boost::thread(threadTCPIP));
    //     threads.push_back(thread);
    // }
    // catch (SockExcept& e) {
    //     cout << "Exception was thrown: "
    //          << e.get_SockExcept() << endl;
    // }

    // Wait for all threads in the pool to exit.
    // for (std::size_t i = 0; i < threads.size(); ++i)
    //     threads[i]->join();

    return ret;
}
