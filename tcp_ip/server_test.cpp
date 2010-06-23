#include <iostream>

#include "serversock.hpp"

using namespace std;

int main() {
   try {
      ServerSock server ( 15000 );
      while ( true ) {
         ServerSock new_sock;
         server.accept ( new_sock );
         try {
            while ( true ) {
               string data;
               new_sock >> data;
               cout << "Nachricht erhalten: " 
                    << data << endl;
               new_sock << "Server hat Daten erhalten";               
               break;
            }
         }
         catch ( SockExcept& ) {}
         new_sock.close();
      }
   }
   catch (SockExcept& e) {
      cout << "Exception wurde ausgeworfen: " 
           << e.get_SockExcept() << endl;
   }
   return 0;
}
