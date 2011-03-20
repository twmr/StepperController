//DNS resolver using boost
//g++ -Wall -lboost_system-mt rs232.cpp
#include <boost/asio.hpp>
#include <iostream>

using boost::asio::serial_port;
int main()
{

  boost::asio::io_service my_io_service;

  //basic_serial_port::serial_port port(my_io_service,"/dev/ttyUSB0");
  serial_port port(my_io_service,"/dev/ttyUSB0");

  port.set_option(boos::asio::serial_port_base::baud_rate(9600) );
  return 0;
}
