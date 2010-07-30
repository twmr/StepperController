#include <cstdlib>
#include <iostream>
#include <cstring>
#include "socket.hpp"
using namespace std;

// Konstruktor
Socket::Socket() : m_sock(0) { }

// Destruktor
Socket::~Socket() {
   if ( is_valid() )
      ::close( m_sock );
}

// Erzeugt das Socket  - TCP
bool Socket::create() {
   m_sock = ::socket(AF_INET,SOCK_STREAM,0);
   if (m_sock < 0) {
      throw SockExcept("Fehler beim Anlegen eines Socket");
   }
   int y=1;
   // Mehr dazu siehe Hinweis am Ende
   setsockopt( m_sock, SOL_SOCKET,
               SO_REUSEADDR, &y, sizeof(int));   
   return true;
}

// Erzeugt das Socket  - UDP
bool Socket::UDP_create() {
   m_sock = ::socket(AF_INET,SOCK_DGRAM,0);
   if (m_sock < 0) {
      throw SockExcept("Fehler beim Anlegen eines Socket");
   }
   return true;
}

// Erzeugt die Bindung an die Serveradresse
// - genauer an einen bestimmten Port
bool Socket::bind( const int port ) {
   if ( ! is_valid() ) {
      return false;
   }
   m_addr.sin_family = AF_INET;
   m_addr.sin_addr.s_addr = INADDR_ANY;
   m_addr.sin_port = htons ( port );

   int bind_return = ::bind ( m_sock,
      ( struct sockaddr * ) &m_addr, sizeof ( m_addr ) );
   if ( bind_return == -1 ) {
      return false;
   }
   return true;     
}

// Teile dem Socket mit, dass VerbindungswÃ¼nsche
// von Clients entgegengenommen werden
bool Socket::listen() const {
   if ( ! is_valid() ) {
      return false;
   }
   int listen_return = ::listen ( m_sock, MAXCONNECTIONS );
   if ( listen_return == -1 ) {
      return false;
   }
  return true;
}

// Bearbeite die Verbindungswünsche von Clients
// Der Aufruf von accept() blockiert solange,
// bis ein Client Verbindung aufnimmt
bool Socket::accept ( Socket& new_socket ) const {
   int addr_length = sizeof ( m_addr );
   new_socket.m_sock = ::accept( m_sock,
      ( sockaddr * ) &m_addr, ( socklen_t * ) &addr_length );
   if ( new_socket.m_sock <= 0 )
      return false;
   else
      return true;
}

// Baut die Verbindung zum Server auf
bool Socket::connect( const string host, const int port ) {
   if ( ! is_valid() )
      return false;      
   struct hostent *host_info;
   unsigned long addr;
   memset( &m_addr, 0, sizeof (m_addr));
   if ((addr = inet_addr( host.c_str() )) != INADDR_NONE) {
       /* argv[1] ist eine numerische IP-Adresse */
       memcpy( (char *)&m_addr.sin_addr,
               &addr, sizeof(addr));
   }
   else {
       /* Für den Fall der Fälle: Wandle den Servernamen  *
        * bspw. "localhost" in eine IP-Adresse um         */
       host_info = gethostbyname( host.c_str() );
       if (NULL == host_info) {
          throw SockExcept("Unbekannter Server");
       }
       memcpy( (char *)&m_addr.sin_addr, host_info->h_addr,
                host_info->h_length);
   }
   m_addr.sin_family = AF_INET;
   m_addr.sin_port = htons( port );      

   int status = ::connect ( m_sock,
      ( sockaddr * ) &m_addr, sizeof ( m_addr ) );

  if ( status == 0 )
    return true;
  else
    return false;
}

// Daten versenden via TCP
bool Socket::send( const string s ) const {
   int status = ::send ( m_sock, s.c_str(), s.size(),  0 );
   if ( status == -1 ) {
      return false;
   }
   else {
      return true;
   }
}

// Daten empfangen via TCP
int Socket::recv ( string& s ) const {
  char buf [ MAXRECV + 1 ];
  s = "";
  memset ( buf, 0, MAXRECV + 1 );
  
  int status = ::recv ( m_sock, buf, MAXRECV, 0 );
  if ( status > 0 || status != -1 ) {
     s = buf;
     return status;
  }
  else {
     throw SockExcept("Fehler in Socket::recv");
     return 0;
  } 
}

// Daten versenden via UDP
bool Socket::UDP_send( const string addr, const string s, const int port ) const {
   struct sockaddr_in addr_sento;
   struct hostent *h;
   int rc;
   
   h = gethostbyname(addr.c_str());
   if (h == NULL) {
      throw SockExcept("Unbekannter Host?");
   }
   addr_sento.sin_family = h->h_addrtype;
   memcpy ( (char *) &addr_sento.sin_addr.s_addr,
            h->h_addr_list[0], h->h_length);
   addr_sento.sin_port = htons (port);
   rc = sendto( m_sock, s.c_str(), s.size(), 0,
                 (struct sockaddr *) &addr_sento,
                  sizeof (addr_sento));
   if (rc == -1) {
      throw SockExcept(
         "Konnte Daten nicht senden - sendto()");

   }        
   return true;
}

// Daten empfangen vie UDP
int Socket::UDP_recv( string& s ) const {
   struct sockaddr_in addr_recvfrom;
   int len, n;
   char buf [ MAXRECV + 1 ];
   s = "";
   memset ( buf, 0, MAXRECV + 1 );
   len = sizeof (addr_recvfrom);
   n = recvfrom ( m_sock, buf, MAXRECV, 0,
                  (struct sockaddr *) &addr_recvfrom,
                  ( socklen_t * )&len );
   if (n == -1){
      throw SockExcept("Fehler bei recvfrom()");
      return 0;
   }
   else {
      s = buf;
      return n;
   } 
}

// Aus Portabiltätsgründen vorhanden
void Socket::cleanup() const { }

// Socket schließen
bool Socket::close() const {
   ::close(m_sock);
   cleanup();
   return true;
}
