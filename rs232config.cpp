#include "rs232.hpp"
#include <stdlib.h>

RS232config::RS232config(const std::string & configfile)
{

    std::ifstream f(configfile.c_str());

    std::cout << "opening RS232 config file: " << configfile << std::endl;
    if( f.is_open() != true) {
	std::cerr << "couldn't open RS232 config file" << std::endl;
	abort();
    }

    f >> devname;
    f >> eos;
    f >> protocol;
    f >> baudrate;
    f >> timeout;
    f >> quit;

    std::cout << "\tDevice name: " << devname  << std::endl;
    std::cout << "\tEOS: " << eos  << std::endl;
    std::cout << "\tProtocol: " << protocol  << std::endl;
    std::cout << "\tBaudrate: " << baudrate  << std::endl;
    std::cout << "\tTimeout: " << timeout  << std::endl;
    std::cout << "\tQuit: " << quit << std::endl;

    f.close();


}
