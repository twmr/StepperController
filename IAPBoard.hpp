#ifndef __IAPBoard__
#define __IAPBoard__

#include "rs232.hpp"
#include "global.hpp"
#include "tcp_ip/serversock.hpp"
#include <mutex>

class IAPBoard {
public:
    static const unsigned int NR_AXIS = 3;  // 3 axis are currently controlled by the IAP Board
    IAPBoard(const RS232config &);
    ~IAPBoard();
    void reset();
    void test(ServerSock&);
    void send_command(ServerSock&,const std::string &);
    void connect();
    void disconnect();
    int getaxisnum(ServerSock&) const;
    int setaxisnum(ServerSock&, const unsigned int);

private:
    bool connected;
    STD_TR1::shared_ptr< RS232 > serial_interface;
    STD_TR1::shared_ptr< std::mutex > boardmutex;

    struct mct_Axis
    {
        long axis_BaseSpeed;
        long axis_SlewSpeed;
        long axis_SlowJogSpeed;
        long axis_FastJogSpeed;
        long axis_CreepSteps;

        long axis_Accel;

        long axis_LowerLimit;
        long axis_UpperLimit;

        long axis_Position;
        char axis_MotorStatus;  /* currently unused */
        double axis_Multiplier;
    } axis[NR_AXIS];

    struct mct_Axis *curaxis;
};

#endif
