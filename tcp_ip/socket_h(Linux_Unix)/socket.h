#ifndef SOCKET_H_
#define SOCKET_H_
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

// Max. Anzahl Verbindungen
const int MAXCONNECTIONS = 5;
// Max. Anzahl an Daten die aufeinmal empfangen werden
const int MAXRECV = 1024;

// Die Klasse Socket
class Socket {
   private:
   // Socketnummer (Socket-Deskriptor)           
   int m_sock; 
   // Struktur sockaddr_in
   sockaddr_in m_addr;
   
   public:
   // Konstruktor
   Socket();
   // virtueller Destruktor
   virtual ~Socket();

   // Socket erstellen - TCP
   bool create();
   // Socket erstellen - UDP
   bool UDP_create();
   bool bind( const int port );
   bool listen() const;
   bool accept( Socket& ) const;
   bool connect ( const string host, const int port );
   // Datenübertragung - TCP
   bool send ( const string ) const;
   int recv ( string& ) const;
   // Datenübertragung - UDP
   bool UDP_send( const string, const string,
                  const int port ) const;
   int UDP_recv( string& ) const;
   // Socket schließen
   bool close() const;
   // WSAcleanup()
   void cleanup() const;
   bool is_valid() const { return m_sock != -1; }
};

// Exception-Klasse
class SockExcept {
   private:
   string except;
   
   public:
   SockExcept( string s ) : except( s ) {};
   ~SockExcept() {};
   string get_SockExcept() { return except; }
};

#endif
