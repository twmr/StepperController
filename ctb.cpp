#include <iostream>

#include "rs232.hpp"
#include "global.hpp"

sctl_ctb::sctl_ctb(const RS232config & config) :
    RS232(config),
    serialPort(STD_TR1::shared_ptr< ctb::SerialPort >(new ctb::SerialPort())),
    device(STD_TR1::shared_ptr< ctb::IOBase >(static_cast<ctb::IOBase*>(0)))
{
}


void sctl_ctb::open() 
{
    rslog("ctb open");

    if( serialPort->Open( rsconfig.get_string_param("devname").c_str(),
			  rsconfig.get_int_param("baudrate"),
			  rsconfig.get_string_param("protocol").c_str(),
			  ctb::SerialPort::NoFlowControl ) >= 0 ) {
	device = serialPort;	
    }

    if( ! device ) {
	std::cerr << "Cannot open " <<  rsconfig.get_string_param("devname")  << std::endl;
	abort();
    }
    std::cout << "opened " <<  rsconfig.get_string_param("devname")  << std::endl;
}


void sctl_ctb::close() 
{
    rslog("ctb close");

    if( device )
	device->Close();
}


size_t sctl_ctb::receive(char *buf, const ssize_t n) 
{
    size_t size = 0;
    //ssize_t totalsize = 0;

    size = device->Read( buf, n);

    // something received?
    if( size > 0 ) {
	//totalsize += size;
	buf[ size ] = 0;
	//std::cout << buf;
	rslog(buf);
    }
    
   
    return size;
}


size_t sctl_ctb::send( const char *buf, const ssize_t n)
{
    size_t size = 0;

    size = device->Write( const_cast<char*>(buf), n);

    // sent string ?
    if( size > 0 ) {
	rslog("#> ", buf);
    }
       
    return size;
}


