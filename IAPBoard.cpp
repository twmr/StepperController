#include <iostream>

#include "IAPBoard.hpp"

IAPBoard::IAPBoard() :
    baud(9600), // for test with  SBC
    //baud(4900), for IAPboard
    connected(false),
    serial_interface(STD_TR1::shared_ptr< RS232 >(new sctl_ctb(baud)))
{
    std::cout << "board: ctor" << std::endl;

    //opens and initializes the serial device
    serial_interface->connect();
}

IAPBoard::~IAPBoard()
{
 std::cout << "board: dtor" << std::endl;

 if(connected)
     disconnect();

}

//activate the Steppercontrolboard
void IAPBoard::connect()
{
     std::cout << "board: connecting to sctlbrd" << std::endl;
}


//shut down the Steppercontrolboard to prevent overheating of the steppermotors
void IAPBoard::disconnect()
{
     std::cout << "board: disconnecting from sctlbrd" << std::endl;
}


void IAPBoard::test()
{
    char buffer[1024];
 
    std::cout << "board: serial test" << std::endl;

    while(serial_interface->receive(buffer, 1024) <= 0 ) {
	std::cout << "board: did not read anything from serial device" << std::endl;
	sleep(1);
    }
    std::cout << buffer;

}

void IAPBoard::reset()
{
 std::cout << "board: reset" << std::endl;

}
