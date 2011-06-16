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

#include <mutex>
#include <map>
#include "global.hpp"
#include "exceptions.hpp"
#include "position.hpp"
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
    int initAxes();
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
    void SetZero();

    size_t GetNrOfAxes(void) const { return NR_AXES; };

    const char *get_err_string(pm381_err_t type) { return pm381_err_string[type]; };
private:
    void send_lowlevel(char * buffer, const size_t size) const;
    static const size_t NR_AXES = 3;  // 3 axis are currently controlled by the IAP Board
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
    } axis[NR_AXES];

    struct mct_Axis *curaxis;
};

#endif
