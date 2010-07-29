#include <iostream>
#include <thread>

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
};

void threadRS232()
{
      std::lock_guard<std::mutex> lk(m);
      //cout << "in RS232" << abc << "\n" << endl;
}

void threadTCPIP()
{
    /*{
	std::lock_guard<std::mutex> lk(m);
	abc[1] = 'L';
	cout << "in TCPIP" << abc << "\n" << endl;
	}
	return;*/
    try {
	ServerSock server ( IAPServer::port );
	while ( true ) {
	    ServerSock new_sock;
	    server.accept ( new_sock );
	    try {
		while ( true ) {
		    string data;
		    new_sock >> data;
		    cout << "Got Command: " 
			 << data << endl;

		    if(! data.compare("help")) {
			new_sock << helpMessage;
			continue;
		    }

		    if(! data.compare("connect")) {
			new_sock << "connecting..";
			// fixme lock this
			board->connect();
			new_sock << "OK";
			continue;
		    }

		    if(! data.compare("disconnect")) {
			new_sock << "disconnecting ..";
			// fixme lock this
			board->disconnect();
			new_sock << "OK";
			continue;
		    }

		    if(! data.compare("serialtest")) {
			// fixme lock this
			board->test(new_sock);
			continue;
		    }

		    if(! data.compare("close")) {
			new_sock << "stopping current session";
			break;
		    }

		    new_sock << "unknown command" << data; // << endl;
		    
		}
         }
         catch ( SockExcept& ) {}
         new_sock.close();
      }
   }
   catch (SockExcept& e) {
      cout << "Exception was thrown: "
           << e.get_SockExcept() << endl;
   }
}


int main(int argc, char** argv)
{
    int ret = 0;
    string configfilename("rsconf");

    if(argc > 1)
	configfilename = argv[1];

    RS232config config(configfilename);

    cout << "SERVER: init IAPBoard" << endl;
    board = STD_TR1::shared_ptr<IAPBoard>(new IAPBoard(config));
    
    cout << "SERVER: starting threads" << endl;
    std::thread trs232(threadRS232);
    std::thread ttcpip(threadTCPIP);

    trs232.join();
    ttcpip.join();

    return ret;
}
