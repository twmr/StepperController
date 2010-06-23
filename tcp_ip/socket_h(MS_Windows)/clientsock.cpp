// clientsock.cpp
#include "clientsock.h"
#include "socket.h"
using namespace std;

ClientSock::ClientSock( string host, int port ) {
   if ( ! Socket::create() ) {
      throw SockExcept (
         "ClientSock: Fehler bei Socket::create()");
   }
   if ( ! Socket::connect ( host, port ) ) {
      throw SockExcept(
         "ClientSock: Fehler bei Socket::connect()");
   }
}

const ClientSock& ClientSock::operator << ( const string& s ) const {
   if ( ! Socket::send ( s ) ) {
      throw SockExcept(
         "ClientSock: Fehler bei Socket::send()");
   }
   return *this;
}

const ClientSock& ClientSock::operator >> ( string& s ) const {
   if ( ! Socket::recv ( s ) ) {
      throw SockExcept(
         "ClientSock: Fehler bei Socket::recv()");
   }
   return *this;
}

void ClientSock::close() const {
   if( ! Socket::close() ) {
      throw SockExcept(
         "ClientSock: Fehler bei Socket::close()");
   }
}
