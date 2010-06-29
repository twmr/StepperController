#ifndef __IAPBoard__
#define __IAPBoard__

#include "RS232.hpp"
#include "global.hpp"
#include <mutex>

class IAPBoard {
public:
    IAPBoard();
    ~IAPBoard();
    void reset();

private:
    int baud;
    STD_TR1::shared_ptr< RS232 > serial_interface;
    STD_TR1::shared_ptr< std::mutex > boardmutex;
};

#endif 
