// clientsock.h
#ifndef CLIENT_SOCK_H
#define CLIENT_SOCK_H
#include <string>
#include "socket.h"
using namespace std;

class ClientSock : private Socket {
   public:
   // Konstruktor
   ClientSock( string host, int port );
   // Destruktor
   virtual ~ClientSock(){};
   // Daten senden
   const ClientSock& operator << ( const string& ) const;
   // Daten empfangen
   const ClientSock& operator >> ( string& ) const;
   // Socket schlieﬂen
   void close() const;   
};
#endif
