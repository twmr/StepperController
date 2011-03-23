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

#include "IAP_server.hpp"
#include "rs232.hpp"
#include "global.hpp"
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
    void test(session&);
    void send_command(session&,const std::string &);
    int send_command_quiet(const std::string &);
    void connect(session&);
    void disconnect();
    int initAxis(session&);
    int getaxisnum() const;
    int setaxisnum(session&, const unsigned int, bool);

    int get_nr_axis(void) const { return NR_AXIS; };

private:
    void send_lowlevel(char * buffer, const size_t size);
    static const unsigned int NR_AXIS = 3;  // 3 axis are currently controlled by the IAP Board
    bool connected;
    STD_TR1::shared_ptr< RS232 > serial_interface;
    STD_TR1::shared_ptr< std::mutex > boardmutex;
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
