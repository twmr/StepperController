#ifndef __RS232__
#define __RS232__

#include "ctb-0.15/ctb.h"
#include "global.hpp"

class RS232 {
public:
    RS232(const int baud) { connected = 0; };
    RS232() { connected = 0; };
    virtual ~RS232() { };

    // pure virtual functions
    virtual int connect() = 0;
    virtual int disconnect() = 0;
    virtual int send() {};
    virtual size_t receive() {};
    virtual size_t receive(char *buf, const ssize_t n) = 0;
    bool is_connected() { return connected; };

private:
    bool connected;
};

//use libcty for rs232
class sctl_ctb: public RS232 {
public:
    sctl_ctb(const int baud);
    sctl_ctb() { std::cout << "false ctor ctb" << std::endl; };
    ~sctl_ctb();
    int connect();
    int disconnect();
    //int send() {};
    //int receive() {};
    size_t receive(char *buf, const ssize_t n);
    
private:
    STD_TR1::shared_ptr< ctb::SerialPort > serialPort;
    STD_TR1::shared_ptr< ctb::IOBase > device;
    std::string devname;
    std::string eos;
    std::string protocol;
    int baudrate;
    int timeout;
    int quit;
};

//use boost asio library for rs232
class sctl_asioboost: public RS232 {
public:
    sctl_asioboost(const int baud);
    sctl_asioboost() { std::cout << "false asio ctb" << std::endl; };
    ~sctl_asioboost();
    int connect();
    int send() {};
    size_t receive() {};
private:
 
    std::string devname;
    std::string eos;
    std::string protocol;
    int baudrate;
    int timeout;
    int quit;
};


#endif 
