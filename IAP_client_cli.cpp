#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>

#include "IAP_server.hpp"
#include "tcp_ip/clientsock.hpp"

using namespace std;

int main (int argc, char *argv[]) {
   if( argc < 2 ){
      cout << "Usage: " << *argv << " server-hostname  [cmd-file]\n";
      exit(1);
   }
   string serverhost = argv[1];
   ifstream f;
   bool read_cmd_list = false;

   if(argc==3) {
       f.open(argv[2]);
       if(f.is_open() != true) {
	   cerr << "couldn't open " <<argv[2] << endl;
	   return 1;
       }
       read_cmd_list = true;
   }

   try {
       cout << "trying to connect to " << serverhost << ":" 
	    << IAPServer::port << endl;
       ClientSock client_socket ( serverhost, IAPServer::port );

       while( true ) {
	   cout << "#> ";
	   string s;
	   if(read_cmd_list) {
	       f >> s;
	       if(f.eof()) break;
	       cout << s << endl;
	   } else
	       getline(cin, s, '\n' );

	   // cout << "got "  << s[0] << s.length() << endl;

	   if(!s.compare("quit") || !s.compare("q")) {
               /* disconnect from tcp/ip server */
	       break; 
	   }

	   if(!s.length()) // empty string
	       continue;

	   try {
	       client_socket << s;
	       client_socket >> s;
	       cout << s <<  endl;
	   }
	   catch ( SockExcept& ) {}
	   
       } // while (1)
       client_socket.close();

   }
   catch ( SockExcept& e ) {
       cout << "Exception: " 
	    << e.get_SockExcept() << endl;
   }

   f.close();
   return 0;   
}
