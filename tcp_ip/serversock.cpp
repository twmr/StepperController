#include "serversock.hpp"
#include "socket.hpp"

ServerSock::ServerSock ( int port ) {
   Socket();
   if ( ! Socket::create() ) {
      throw SockExcept (
         "ServerSock: Fehler bei Socket::create()");
   }
   if ( ! Socket::bind ( port ) ) {
      throw SockExcept(
         "ServerSock: Fehler bei Socket::bind()");
   }
   if ( ! Socket::listen() ) {
      throw SockExcept(
         "ServerSock: Fehler bei Socket::listen()");
   }
}

ServerSock::~ServerSock() { }

const ServerSock&
ServerSock::operator << ( const std::string& s ) const {
   if ( ! Socket::send ( s ) ) {
      throw SockExcept (
         "ServerSock: Fehler bei Socket::send()");
   }
   return *this;
}

const ServerSock& ServerSock::operator >> ( std::string& s ) const {
   if ( ! Socket::recv ( s ) ) {
      throw SockExcept(
         "ServerSock: Fehler bei Socket::recv()");
   }
   return *this;
}

void ServerSock::accept ( ServerSock& sock ) {
   if ( ! Socket::accept ( sock ) ) {
      throw SockExcept(
         "ServerSock: Fehler bei Socket::accept()");
   }
}

void ServerSock::close() const {
   if( ! Socket::close() ) {
      throw SockExcept(
         "ServerSock: Fehler bei Socket::close()");
   }
}
