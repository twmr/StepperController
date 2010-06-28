#include <iostream>
#include <thread>

#include "IAPServer.hpp"
#include "tcp_ip/serversock.hpp"
//#include "RS232.hpp"
#include "IAPBoard.hpp"

using namespace std;

string abc("Mongo");
mutex m;
STD_TR1::shared_ptr<IAPBoard> board(static_cast<IAPBoard*>(0));//(nullptr);

void threadRS232()
{
      std::lock_guard<std::mutex> lk(m);
      abc[0] = 'A';
      cout << "in RS232" << abc << "\n" << endl;
      cout << "RS232: iapboard ctor\n" << endl;
      board = STD_TR1::shared_ptr<IAPBoard>(new IAPBoard);
      cout << "RS232: end iapboard ctor\n" << endl;     
}

void threadTCPIP()
{
    {
	std::lock_guard<std::mutex> lk(m);
	abc[1] = 'L';
	cout << "in TCPIP" << abc << "\n" << endl;
    }
    return;
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
		    new_sock << "Ack";               
		    break;
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
    
    std::thread trs232(threadRS232);
    std::thread ttcpip(threadTCPIP);

    trs232.join();
    ttcpip.join();

    return ret;
}
