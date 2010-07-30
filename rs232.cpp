#include "rs232.hpp"
#include <ctime>

RS232::RS232(const RS232config & config) :
    myfilestream("rs232.log", std::ios::out | std::ios::app),
    rsconfig(config)
{
    if(myfilestream.is_open() != true) {
	std::cout << "couldn't open rs232 log file" << std::endl;
	abort();
    }
};


int RS232::rslog(const std::string & logstring, const std::string prefix)
{
    struct tm *sec;
    std::time_t mytime;
    char tmp[16];

    std::time(&mytime);

    sec = localtime(&mytime);
    strftime(tmp,16, "%y%m%02d %H:%M:%S", sec);

    myfilestream << tmp << " | "<< prefix << logstring << std::endl;
    myfilestream << std::flush;
}
