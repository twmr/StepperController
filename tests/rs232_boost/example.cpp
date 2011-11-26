#include <string>
#include <iostream>
#include <exception>
#include <boost/asio.hpp>
#include <boost/system/system_error.hpp>

int main(int argc, char *argv[])
{
    using namespace boost::system;
    using namespace boost::asio;

    io_service ioService;
    serial_port serialPort(ioService);
    size_t n = 200;

    try
    {
        serialPort.open("/dev/ttyUSB0");

        serialPort.set_option(serial_port::baud_rate(9600));
        serialPort.set_option(serial_port::parity(serial_port::parity::none));
        serialPort.set_option(serial_port::character_size(serial_port::character_size(8)));
        serialPort.set_option(serial_port::stop_bits(serial_port::stop_bits::one));
        serialPort.set_option(serial_port::flow_control(serial_port::flow_control::none));

        std::vector<char> packed_dimensions(n);
        boost::system::error_code e;
        size_t len = 0;

        //SBC3 TEST:
        packed_dimensions[0]='h';
        packed_dimensions[1]='e';
        packed_dimensions[2]='l';
        packed_dimensions[3]='p';
        packed_dimensions[4]='\r';
        packed_dimensions[5]='\n';
        packed_dimensions[6]='\0';
        len = serialPort.write_some(boost::asio::buffer(&packed_dimensions[len], 6), e);
        std::cout << "wrote" << len <<" bytes" << std::endl;

        len =0;
        do  {
            len += serialPort.read_some(boost::asio::buffer(&packed_dimensions[len], n-len), e);
        } while ( (len < n) && (!e) );

        if (len == n) {
            std::cout << std::string(packed_dimensions.begin(), packed_dimensions.end()) << std::endl;
        } else {
            //throw std::exception(e.message().c_str());
        }

        serialPort.close();
    }
    catch (system_error& error)
    {
        std::cout << "Exception: " << error.what() << std::endl;
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

}
