#ifndef __IAPBoard__
#define __IAPBoard__

#include "RS232.hpp"
#include "global.hpp"
#include "tcp_ip/serversock.hpp"
#include <mutex>

class IAPBoard {
public:
    IAPBoard(const RS232config & config);
    ~IAPBoard();
    void reset();
    void test(ServerSock&);
    void send_command(ServerSock&,const std::string &);
    void connect();
    void disconnect();

private:
    bool connected;
    STD_TR1::shared_ptr< RS232 > serial_interface;
    STD_TR1::shared_ptr< std::mutex > boardmutex;
};

#endif 
