#include <iostream>
//#include <thread>
#include <boost/thread.hpp>
#include <boost/regex.hpp>


#include "IAP_server.hpp"
#include "tcp_ip/serversock.hpp"
#include "IAPBoard.hpp"

using namespace std;

mutex m;
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

void threadRS232()
{
    std::lock_guard<std::mutex> lk(m);
    //cout << "in RS232" << abc << "\n" << endl;
}

void threadTCPIP()
{
    // {
    //     std::lock_guard<std::mutex> lk(m);
    //     abc[1] = 'L';
    //     cout << "in TCPIP" << abc << "\n" << endl;
    // }
    // return;


    ServerSock server ( IAPServer::port );
    ServerSock client;
    server.accept ( client );
    try {
        while ( true ) {
            string data;
            client >> data;
            cout << "Got Command: " << data << endl;

            if(! data.compare("help")) {
                client << helpMessage;
                continue;
            }

            if(! data.compare("connect")) {
                client << "connecting..";
                // fixme lock this
                board->connect(client);
                continue;
            }

            if(! data.compare("disconnect")) {
                client << "disconnecting ..";
                // fixme lock this
                board->disconnect();
                client << "OK";
                continue;
            }

            if(! data.compare("serialtest")) {
                // fixme lock this
                board->test(client);
                continue;
            }

            if(! data.compare("close")) {
                client << "stopping current session";
                break;
            }

            if(! data.compare("enjog")) {
                //enable jog mode
                board->send_command(client, "1AR");
                continue;
            }
            if(! data.compare("disjog")) {
                //disable jog mode
                board->send_command(client, "1IR");
                continue;
            }

            if(! data.compare("pp")) {
                //print positions
                int pos[board->get_nr_axis()];
                char buf[80];
                int oldaxisnum = board->getaxisnum();

                for (int i = 0; i < board->get_nr_axis(); ++i)
                {
                    board->setaxisnum(client,i,true);
                    pos[i] = board->send_command_quiet("1OC");
                }

                board->setaxisnum(client,oldaxisnum,true);
                sprintf(buf," axis1: %d\n axis2: %d\n axis3: %d", pos[0], pos[1], pos[2]);
                client << buf;
                continue;
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
                board->setaxisnum(client,axisnum,false);
                continue;
            }
            boost::regex e2("^[[:digit:]][[:alpha:]]+-?[[:digit:]]*$");

            if(boost::regex_match(data, what, e2, boost::match_extra))
                board->send_command(client, data);
            else
                client << "invalid command (check syntax)";

        }
    }
    catch ( SockExcept& ) {
    }

    client.close();
}

int main(int argc, char** argv)
{
    int ret = 0;
    string serialconfigfilename("rsconf");
    string axisconfigfilename("axis.cfg");
    std::vector<boost::shared_ptr<boost::thread> > threads;

    if(argc > 1)
        axisconfigfilename = argv[1];

    RS232config rsconfig(serialconfigfilename);
    IAPconfig axisconfig(axisconfigfilename);

    cout << "SERVER: init IAPBoard" << endl;
    board = STD_TR1::shared_ptr<IAPBoard>(new IAPBoard(rsconfig,axisconfig));

    try {
    // ServerSock server ( IAPServer::port );
    //     while ( true ) {
    //         ServerSock new_sock;
    //         server.accept ( new_sock );
    //         new_sock << "lulu";
    //         cout << "SERVER: client connected: starting new tcpip thread" << endl;
    //     }
        boost::shared_ptr<boost::thread> thread(new boost::thread(threadTCPIP));
        threads.push_back(thread);
    }
    catch (SockExcept& e) {
        cout << "Exception was thrown: "
             << e.get_SockExcept() << endl;
    }

    // Wait for all threads in the pool to exit.
    for (std::size_t i = 0; i < threads.size(); ++i)
        threads[i]->join();

    return ret;
}
