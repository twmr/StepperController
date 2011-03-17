#ifndef __IAPBoard__
#define __IAPBoard__

#include "rs232.hpp"
#include "global.hpp"
#include "tcp_ip/serversock.hpp"
#include <mutex>


class IAPconfig {
public:
    IAPconfig() {};
    IAPconfig(const std::string & configfile);
    ~IAPconfig() {};

    const char* name() const { return "rs232";};

    bool get_param(const std::string& s, double& v) const;
    bool get_param(const std::string& s, int& v) const;
    bool get_param(const std::string& s, std::string& v) const;

    double      get_double_param(const std::string& s) const throw (E_missing_parameter);
    int         get_int_param(const std::string& s) const throw (E_missing_parameter);
    std::string get_string_param(const std::string& s) const throw (E_missing_parameter);

private:
    //std::map<std::string, double> r_params;
    std::map<std::string, std::string> s_params;
    //std::map<std::string, int> i_params;
};


class IAPBoard {
public:
    IAPBoard(const RS232config &, const IAPconfig &);
    ~IAPBoard();
    void reset();
    void test(ServerSock&);
    void send_command(ServerSock&,const std::string &);
    int send_command_quiet(const std::string &);
    void connect(ServerSock& );
    void disconnect();
    int initAxis(ServerSock&);
    int getaxisnum() const;
    int setaxisnum(ServerSock&, const unsigned int, bool);

    int get_nr_axis(void) const { return NR_AXIS; };

private:
    static const unsigned int NR_AXIS = 3;  // 3 axis are currently controlled by the IAP Board
    bool connected;
    STD_TR1::shared_ptr< RS232 > serial_interface;
    STD_TR1::shared_ptr< std::mutex > boardmutex;
    const IAPconfig &axisconfig;

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
