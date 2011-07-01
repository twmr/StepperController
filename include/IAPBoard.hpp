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
#include <boost/property_tree/ptree.hpp>
#include <boost/foreach.hpp>
#include "global.hpp"
#include "exceptions.hpp"
#include "position.hpp"
#include <iostream>

typedef enum{
    E_DUMMY=0,
    E_PM381_ERROR_MSG,
    E_SIZE_PM381_REPLY_SHORT,
    E_AXIS_NUM_INVALID,
    E_LAST
} pm381_err_t;

static const char* pm381_err_string[E_LAST] = {
    "this should't be printed - probably a bug in this software",
    "PM381 returned error message",
    "size of reply from stepper card was too short",
    "Axis Number is not valid"
};

/* forward declatrions */
class RS232;

class IAPconfig {
public:
    IAPconfig(const std::string & configfile);
    ~IAPconfig() {
        system("rm lala");
//        std::cout << "LLLLLLLLLLLLLLLLLLL" << std::endl;
    };

    void writeconfig() const;

    const boost::property_tree::ptree& GetParameters(const std::string& group) const {
        std::string pN = "config." + group;
        return params_.get_child(pN);
    }

    const boost::property_tree::ptree& GetParameters() const {
        return params_.get_child("config");
    }

    template <class T>
    const T get(const std::string &s) const { return params_.get<T>(s); };

    template <class T>
    void set(const std::string &s, T value) const { return params_.put(s, value); };

    template <class T>
    void setAxisElement(const size_t axnr, const std::string& element, const T value) {
        bool found = false;
        BOOST_FOREACH(boost::property_tree::ptree::value_type & v,
                      params_.get_child("config")) {
            if(v.first.data() != std::string("axis")) continue;

            size_t id = v.second.get<size_t>("<xmlattr>.id");
            std::cout << id << "found" << std::endl;
            if( id != axnr )
                continue;

            found = true;
            v.second.put(element, value);
        }

        if(!found)
            std::cerr << "WARNING: element (axis: "<< axnr << ") " << element
                      << "not found in property tree" << std::endl;
    };


private:
    boost::property_tree::ptree params_;
    const std::string filename;
};


class IAPBoard {
public:
    IAPBoard(IAPconfig &);
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

    IAPconfig& getConfig() const { return config_; };

    bool is_connected() { return connected; };
    int SetZero();

    size_t GetNrOfAxes(void) const { return NR_AXES; };

    const char *get_err_string(pm381_err_t type) {
        if(type >= E_LAST || type < 0)
            return "BUG: error string not valid";
        else
            return pm381_err_string[type]; };
private:
    std::string send_lowlevel(const std::string&) const;
    static const size_t NR_AXES = 3;  // 3 axis are currently controlled by the IAP Board
    bool connected;
    STD_TR1::shared_ptr< RS232 > serial_interface;
    STD_TR1::shared_ptr< std::mutex > boardmutex;
    STD_TR1::shared_ptr<ConversionConstants> convconsts;
    IAPconfig &config_;

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
