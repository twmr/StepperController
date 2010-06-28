#include <iostream>

#include "IAPBoard.hpp"

IAPBoard::IAPBoard() :
    baud(4900),
    serial_interface(STD_TR1::shared_ptr< RS232 >(new sctl_ctb(baud)))
{
 std::cout << "board ctor" << std::endl;
 
 serial_interface->connect();
 
}

IAPBoard::~IAPBoard()
{
 std::cout << "board dtor" << std::endl;

}

void IAPBoard::reset()
{
 std::cout << "board reset" << std::endl;

}
