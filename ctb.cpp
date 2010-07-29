#include <iostream>

#include "RS232.hpp"
#include "global.hpp"

sctl_ctb::sctl_ctb(const int baud) :
    serialPort(STD_TR1::shared_ptr< ctb::SerialPort >(new ctb::SerialPort())) ,
    device(STD_TR1::shared_ptr< ctb::IOBase >(static_cast<ctb::IOBase*>(0))) ,
    devname("/dev/ttyUSB0"),
    eos("\r\n"),
    protocol("8N1")
{
    std::cout << "ctb ctor" << std::endl;

    timeout = 100;
    quit = 0;
    baudrate = baud;
}

sctl_ctb::~sctl_ctb() {
    std::cout << "ctb dtor" << std::endl;
}


// we should call this function open instead of connect
int sctl_ctb::connect() 
{
    std::cout << "ctb connect" << std::endl;

    if( serialPort->Open( devname.c_str(), baudrate, 
			  protocol.c_str(), 
			  ctb::SerialPort::NoFlowControl ) >= 0 ) {
	device = serialPort;	
    }

    if( ! device ) {
	std::cerr << "Cannot open " << devname.c_str() << std::endl;
	return -1;
    }

    return 0;
}

int sctl_ctb::disconnect() 
{
    std::cout << "ctb disconnect" << std::endl;

    return 0;
}


size_t sctl_ctb::receive(char *buf, const ssize_t n) 
{
    size_t size = 0;
    //ssize_t totalsize = 0;
    //int timeout = 100;

    size = device->Read( buf,
			 n);

    // something received?
    if( size > 0 ) {
	//totalsize += size;
	buf[ size ] = 0;
	//std::cout << buf;
    }
    return size;
}


