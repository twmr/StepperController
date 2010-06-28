#include <iostream>

#include "RS232.hpp"
#include "global.hpp"

sctl_ctb::sctl_ctb(const int baud) :
    serialPort(STD_TR1::shared_ptr< ctb::SerialPort >(new ctb::SerialPort())) ,
    device(STD_TR1::shared_ptr< ctb::IOBase >(static_cast<ctb::IOBase*>(0))) ,
    devname(ctb::COM1),
    eos("\r\n"),
    protocol("8N1")
{
    std::cout << "ctb ctor" << std::endl;

    timeout = 100;
    quit = 0;
    baudrate = baud;

    if( serialPort->Open( devname.c_str(), baudrate, 
			  protocol.c_str(), 
			  ctb::SerialPort::NoFlowControl ) >= 0 ) {
	device = serialPort;	
    }

    if( ! device ) {
	std::cout << "Cannot open " << devname.c_str() << std::endl;
	return;
    }

}

sctl_ctb::~sctl_ctb() {
    std::cout << "ctb dtor" << std::endl;
}

int sctl_ctb::connect() {
    std::cout << "ctb connect" << std::endl;
    return 0;
}


