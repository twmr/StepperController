/*
 * IAPBoard.hpp
 *
 * Copyright (c) 2010-2011 Thomas Hisch <thomas at opentech dot at>.
 *
 * This file is part of StepperControl.
 *
 * StepperControl is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * StepperControl is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with StepperControl.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __IAPBoard__
#define __IAPBoard__

#include <boost/tuple/tuple.hpp>
#include <mutex>
#include <map>
#include "global.hpp"
#include "exceptions.hpp"
#include <iostream>

typedef enum{
    E_DUMMY=0,
    E_PM381_ERROR_MSG,
    E_SIZE_PM381_REPLY_SHORT,
    E_LAST
} pm381_err_t;

static const char* pm381_err_string[E_LAST] = {
    "this should't be printed - probably a bug in this software",
    "PM381 returned error message",
    "size of reply from stepper card was too short"
};

/* forward declatrions */
class RS232;
class RS232config;


template <typename T>
class DummyPosition
{
public:
    DummyPosition()
        { };

    DummyPosition(T x, T y, T t) :
        x_(x), y_(y), theta_(t)
        { };

    DummyPosition(const boost::tuple<T,T,T>& tp) :
        x_(tp.get(0)), y_(tp.get(1)), theta_(tp.get(2))
        { };

    DummyPosition(const std::vector<T>& tvec) :
        x_(tvec[0]), y_(tvec[1]), theta_(tvec[2])
        { };

    void PrintPosition() const
        {
            std::cout << "Position: ( x: " << x_ << " y: " << y_ << " theta: "
                      << theta_ << " )" << std::endl;
        };


    typedef T type;
    T x_;
    T y_;
    T theta_;
};

typedef DummyPosition<short> BarePosition; //units that the stepper board understands
typedef DummyPosition<float> Position; //units that we understand

class ConversionConstants
{
public:
    ConversionConstants() :
        xconv_(1.0), yconv_(1.0), thetaconv_(1.0)
        { };

    ConversionConstants(double xc, double yc, double thetac) :
        xconv_(xc), yconv_(yc), thetaconv_(thetac)
        { };

    void get_bare_position(BarePosition& ret, const Position &pos) const;
    void get_position(Position& ret, const BarePosition &bpos) const;
    void set_constants(const std::vector<double>& cvec);
    double xconv_;
    double yconv_;
    double thetaconv_;
};


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

    int send_command(const std::string &, char* reply) const;
    int send_command_quiet(const std::string &) const;
    int send_getint_command(const std::string &) const;

    void reset();
    void test(char*);
    void connect();
    void disconnect();
    int initAxis();
    int getaxisnum() const;
    int setaxisnum(const unsigned int);
    STD_TR1::shared_ptr<ConversionConstants> getConversionConstants() const
        { return convconsts; };

    void move_rel(const Position& deltaPos);
    void move_to(const Position& absPos);
    void move_rel(const BarePosition& deltaPos);
    void move_to(const BarePosition& absPos);

    void get_cur_position(BarePosition& retbarepos) const;
    void get_cur_position(Position& retpos) const;

    bool is_connected() { return connected; };

    int get_nr_axis(void) const { return NR_AXIS; };
    const char *get_err_string(pm381_err_t type) { return pm381_err_string[type]; };
private:
    void send_lowlevel(char * buffer, const size_t size) const;
    static const unsigned int NR_AXIS = 3;  // 3 axis are currently controlled by the IAP Board
    bool connected;
    STD_TR1::shared_ptr< RS232 > serial_interface;
    STD_TR1::shared_ptr< std::mutex > boardmutex;
    STD_TR1::shared_ptr<ConversionConstants> convconsts;
    const IAPconfig &axisconfig;

    struct mct_Axis {
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
