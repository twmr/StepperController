#include <iostream>
#include <cstdio>

#include "tcp_ip/serversock.hpp"
#include "IAPBoard.hpp"

IAPBoard::IAPBoard(const RS232config & config) :
    connected(false),
    serial_interface(STD_TR1::shared_ptr< RS232 >(new sctl_ctb(config))),
    curaxis(&axis[0])
{
    /* opens/initializes the serial device */
    serial_interface->open();
}

IAPBoard::~IAPBoard()
{
    if(connected)
        disconnect();
}

//activate the Steppercontrolboard
void IAPBoard::connect()
{
     std::cout << "board: connecting to sctlbrd" << std::endl;

     /* connection commands */
     /* TODO */

     connected = true;

}


//shut down the Steppercontrolboard to prevent overheating of the steppermotors
void IAPBoard::disconnect()
{
     std::cout << "board: disconnecting from sctlbrd" << std::endl;

     /* disconnection commands */
     /* TODO */

     connected = false;
}


void IAPBoard::send_command(ServerSock &sock, const std::string & cmd)
{
    char buffer[1024];
    
    cout << "send cmd called " <<  cmd << endl;
    serial_interface->rslog(cmd, "#>");

    string tosend = cmd + "\r\n"; /* maybe not needed */
    while(serial_interface->send(tosend.c_str(), tosend.length()) <= 0 ) {
        sleep(1);
    }

    while(serial_interface->receive(buffer, sizeof(buffer)) <= 0 ) {
        sleep(1);
    }

    sock << buffer;

}

int IAPBoard::getaxisnum(ServerSock& sock) const
{
    return (curaxis-axis)/sizeof(struct mct_Axis);
}

static string binary( unsigned long n )
{
    char     result[ 4 + 1 ];
    unsigned index  = 4;
    result[ index ] = '\0';

    do {
        result[ --index ] = '0' + (n & 1);
        n >>= 1;
    } while (index);

    return string(result);
}

int IAPBoard::setaxisnum(ServerSock& sock, const unsigned int axisnum)
{
    unsigned int curaxisnum  = (curaxis-axis)/sizeof(struct mct_Axis);

    cout << "curaxis " << curaxisnum << " axisnum " << axisnum << endl;
    
    //if (axisnum == curaxisnum) return - 1;

    if(axisnum >= NR_AXIS) {
        cout << " axnium "  << axisnum << " is not a valid axinum" << endl;
        return -EINVAL;
    }

    send_command(sock, "1WP"+binary(axisnum));

    curaxis = &axis[axisnum];
    return curaxisnum;
}

void IAPBoard::test(ServerSock &sock)
{
    char buffer[1024];

    //waits for incoming dota, if no data arrives blocks forever
    serial_interface->rslog(" serial test", "#>");

    while(serial_interface->receive(buffer, 1024) <= 0 ) {
        sleep(1);
    }
    sock << buffer;
}

void IAPBoard::reset()
{
 std::cout << "board: reset" << std::endl;
}
