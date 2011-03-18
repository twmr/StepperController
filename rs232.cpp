#include "rs232.hpp"
#include <ctime>

RS232::RS232(const RS232config & config) :
    rsconfig(config),
    myfilestream("rs232.log", std::ios::out | std::ios::app)
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
    char buf[16];

    std::time(&mytime);

    sec = localtime(&mytime);
    strftime(buf,16, "%y%m%02d %H:%M:%S", sec);

    myfilestream << buf << " | "<< prefix << logstring << std::endl;
    myfilestream << std::flush;

    return 0;
}
