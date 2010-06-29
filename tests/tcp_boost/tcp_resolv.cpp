//DNS resolver using boost
//g++ -Wall -lboost_system-mt tcp_resolv.cpp
#include <boost/asio.hpp>
#include <iostream>

using boost::asio::ip::tcp;
int main()
{

  boost::asio::io_service my_io_service;

tcp::resolver resolver(my_io_service);
tcp::resolver::query query("www.boost.org", "http");
tcp::resolver::iterator iter = resolver.resolve(query);
tcp::resolver::iterator end; // End marker.
while (iter != end)
{
  tcp::endpoint endpoint = *iter++;
  std::cout << endpoint << std::endl;
}
return 0;
}
