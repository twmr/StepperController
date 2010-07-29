#include "RS232.hpp"

RS232::RS232(const RS232config & config) :
    myfilestream("rs232.log"),
    rsconfig(config)
{
    if(myfilestream.is_open() != true) {
	std::cout << "couldn't open rs232 log file" << std::endl;
	abort();
    }
};


int RS232::rslog(const std::string & logstring)
{
    myfilestream << logstring << std::endl;
    myfilestream << std::flush;
}
