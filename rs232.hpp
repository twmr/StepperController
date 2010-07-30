#ifndef __RS232__
#define __RS232__

#include <iostream>
#include <fstream>
#include <stdlib.h>


#include "ctb-0.15/ctb.h"
#include "global.hpp"



class RS232config {
public:
    RS232config() {};
    RS232config(const std::string & configfile);
    ~RS232config() {};

    const std::string & get_devname() const { return devname; };
    const std::string & get_eos() const { return eos; };
    const std::string & get_protocol() const { return protocol; };
    int get_baudrate() const { return baudrate; };
    int get_timeout() const { return timeout; };
    int get_quit() const { return quit; };

private:
    std::string devname;
    std::string eos;
    std::string protocol;
    int baudrate;
    int timeout;
    int quit;
};

class RS232 {
public:
    RS232(const RS232config & config);
    virtual ~RS232() { if(myfilestream.is_open()) myfilestream.close(); };

    // pure virtual functions
    virtual void open() = 0;
    virtual void close() = 0;
    virtual size_t send(const char *buf, ssize_t n) = 0;
    virtual size_t receive(char *buf, const ssize_t n) = 0;

    int rslog(const std::string & logstring, const std::string prefix = "");
    const RS232config &rsconfig;

private:
    std::ofstream myfilestream;
};

//use libctb for rs232
class sctl_ctb: public RS232 {
public:
    sctl_ctb(const RS232config & config);
    ~sctl_ctb() {};
    void open();
    void close();
    //int send() {};
    //int receive() {};
    size_t send(const char *buf, const ssize_t n);
    size_t receive(char *buf, const ssize_t n);
    
private:
    STD_TR1::shared_ptr< ctb::SerialPort > serialPort;
    STD_TR1::shared_ptr< ctb::IOBase > device;
    
};

#endif 
