// echo_client2.cpp
#include "clientsock.h"
#include <string>
#include <iostream>
#include <cstdlib>
using namespace std;

int main (int argc, char *argv[]) {
   if( argc < 2 ){
      cout << "Usage: " << *argv << " ServerAdresse\n";
      exit(1);
   }
   string argv_1 = argv[1];
   try {
      ClientSock client_socket ( argv_1, 15000 );
      cout << "Nachricht an den Server: ";
      string s;
      getline(cin, s, '\n' );
      try {
         client_socket << s;
         client_socket >> s;
         cout << s <<  endl;
      }
      catch ( SockExcept& ) {}
      client_socket.close();
   }
   catch ( SockExcept& e ) {
      cout << "Ein Exception wurde abgefangen: " 
           << e.get_SockExcept() << endl;
   }
   return 0;   
}
