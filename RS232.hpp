
#ifndef __RS232__
#define __RS232__

#include "ctb-0.15/ctb.h"
#include "global.hpp"

class RS232 {
public:
    RS232(const int baud) { connected = 0; };
    RS232() { connected = 0; };
    virtual ~RS232() { };
    virtual int connect() { };
    virtual int send() {};
    virtual int revceive() {};
    bool is_connected() { return connected; };

private:
    bool connected;
};

class sctl_ctb: public RS232 {
public:
    sctl_ctb(const int baud);
    sctl_ctb() { std::cout << "false ctor ctb" << std::endl; };
    ~sctl_ctb();
    int connect();
    int send() {};
    int receive() {};
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


#endif 
