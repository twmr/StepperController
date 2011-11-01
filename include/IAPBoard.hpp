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
#include "position.hpp"
#include <iostream>
#include <stdexcept>

typedef enum{
    E_DUMMY=0,
    E_PM381_ERROR_MSG,
    E_SIZE_PM381_REPLY_SHORT,
    E_AXIS_NUM_INVALID,
    E_LAST
} pm381_err_t;

static const std::string pm381_err_string[E_LAST] = {
    "this should't be printed - probably a bug in this software",
    "PM381 returned error message",
    "size of reply from stepper card was too short",
    "Axis Number is not valid"
};

/* forward declatrions */
class RS232;
class IAPBoard;

class Axis
{
public:
    Axis(const size_t id, const std::string desc, IAPBoard*);
    ~Axis() {};
    size_t get_id() const { return ID_; };
    std::string get_desc() const { return Desc_; };
    std::string get_unitname() const { return UnitName_; };
    void SetOffset(BarePosition::type val) { Offset_ = val; };
    BarePosition::type GetOffset() const { return Offset_; };
    double get_factor() const { return UnitFactor_; };
    void printAxis(void) const;
    int UpdateConfiguration(void);

    int move_rel(const BarePosition::type) const;
    int move_abs(const BarePosition::type) const;

private:
    void parse_vars(const boost::property_tree::ptree & pt);

    size_t ID_;
    std::string Desc_;

    std::string UnitName_;
    double UnitFactor_;
    BarePosition::type Offset_;
    long BaseSpeed_;
    long SlewSpeed_;
    long SlowJogSpeed_;
    long FastJogSpeed_;
    long CreepSteps_;
    long Accel_;
    BarePosition::type LowerLimit_;
    BarePosition::type UpperLimit_;
    BarePosition::type Position_;

    //TODO smart ptr ?!?!
    IAPBoard* Board_;
};



class IAPconfig {
public:
    IAPconfig(const std::string & configfile);
    ~IAPconfig() { };

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

    const boost::property_tree::ptree& getaxispt(const size_t id) const {
        BOOST_FOREACH(const boost::property_tree::ptree::value_type & v,
                      params_.get_child("config")) {
            if(v.first.data() != std::string("axis"))
                continue;

            size_t axnr = v.second.get<size_t>("<xmlattr>.id");
            if( id == axnr )
                return v.second;
        }

        std::cerr << "warning: (axis: "<< id << ") "
                  << "not found in property tree" << std::endl;
        throw std::invalid_argument("bad axis id");
    }


    template <class T>
    void setAxisElement(const size_t axnr, const std::string& element, const T value) {
        bool found = false;
        BOOST_FOREACH(boost::property_tree::ptree::value_type & v,
                      params_.get_child("config")) {
            if(v.first.data() != std::string("axis")) continue;

            size_t id = v.second.get<size_t>("<xmlattr>.id");
            //std::cout << id << "found" << std::endl;
            if( id != axnr )
                continue;

            found = true;
            v.second.put(element, value);
        }

        if(!found)
            std::cerr << "WARNING: element (axis: "<< axnr << ") " << element
                      << "not found in property tree" << std::endl;
    };

    template <class T>
    const T getAxisElement(const size_t axnr, const std::string& element) const {
        bool found = false;
        T retval = 0;
        BOOST_FOREACH(const boost::property_tree::ptree::value_type & v,
                      params_.get_child("config")) {
            if(v.first.data() != std::string("axis")) continue;

            size_t id = v.second.get<size_t>("<xmlattr>.id");
            if( id != axnr )
                continue;

            found = true;
            retval = v.second.get<T>(element);
        }

        if(!found)
            std::cerr << "WARNING: element (axis: "<< axnr << ") " << element
                      << "not found in property tree" << std::endl;
        return retval;
    };


private:
    boost::property_tree::ptree params_;
    const std::string filename;
};


class IAPBoard {
public:
    IAPBoard(IAPconfig &, bool);
    ~IAPBoard();

    int send_command(const std::string &, char* reply) const;
    int send_command_quiet(const std::string &) const;
    int send_getint_command(const std::string &) const;

    void reset();
    void connect();
    void disconnect();
    int initAxes();
    size_t GetAxisID() const;
    int setaxisnum(const size_t);

    int move_rel(const Position& deltaPos) const;
    int move_to(const Position& absPos) const;
    int move_rel(const BarePosition& deltaPos) const;
    int move_to(const BarePosition& absPos) const;

    void get_cur_position(BarePosition& retbarepos) const;
    void get_cur_position(Position& retpos) const;

    void get_upper_softlimits(Position& retsl) const;
    void get_lower_softlimits(Position& retsl) const;

    IAPconfig& getConfig() const { return config_; };

    bool is_connected() const { return connected_; };
    bool state() const { return init_; };

    int SetZero();
    int SetInitialZero();

    size_t GetNrOfAxes(void) const { return axes.size(); };

    BarePosition createBarePosition(void) const;
    Position createPosition(void) const;

    // conversion stuff
    void conv2bareposition(BarePosition& ret, const Position &pos) const;
    void conv2postion(Position& ret, const BarePosition &bpos) const;

    // Axis* getAxis(const size_t id);
    // Axis* getAxis(const std::string desc);
    const Axis * getAxis(const size_t id) const;
    const Axis * getAxis(const std::string desc) const;

    void SaveEnvironment() const;
    void RestoreEnvironment() const;

    std::string get_err_string(pm381_err_t type, char* error_msg=NULL) {
        if(type >= E_LAST || type < 0)
            return std::string("BUG: error string not valid");
        else if (type == E_PM381_ERROR_MSG && error_msg)
            return error_msg;
        else
            return pm381_err_string[type]; };

    typedef std::vector<Axis>::iterator axesiter;
    typedef std::vector<Axis>::const_iterator const_axesiter;

    // const std::map<size_t, std::string>& get_coord_map() const
    //     { return coordinate_map; };

    // const std::map<size_t, std::string>& get_unit_map() const
    //     { return unit_map; };

    // const std::map<std::string, size_t>& get_inv_coord_map() const
    //     { return inv_coordinate_map; };
    std::map<size_t, std::string>& get_coord_map()
        { return coordinate_map; };

    std::map<size_t, std::string>& get_unit_map()
        { return unit_map; };

    std::map<std::string, size_t>& get_inv_coord_map()
        { return inv_coordinate_map; };

    // TODO combine this with the get_err_string() stuff
    const std::string& get_latest_error() const {  return latest_error_str_; };


private:
    int save_setaxisnum(const size_t) const;
    std::string strip_ctrl_junk(const std::string&) const;
    std::string send_lowlevel(const std::string&) const;

    bool init_;
    bool connected_;
    STD_TR1::shared_ptr< RS232 > serial_interface;

    //TODO use this mutex
    // STD_TR1::shared_ptr< std::mutex > boardmutex;
    IAPconfig &config_;
    std::map<size_t, std::string> coordinate_map;
    std::map<size_t, std::string> unit_map;
    std::map<std::string, size_t> inv_coordinate_map;
    // std::map<size_t, BarePosition::type> bp_coordiante_map;
    // std::map<size_t, Position::type> p_coordiante_map;

    mutable struct Environment
    {
        size_t axis_id;
    } envion;

    mutable std::string latest_error_str_;
    std::vector<Axis> axes;
    Axis *curaxis;
    mutable Axis *save_curaxis;

    Position iap_default_position;
    BarePosition iap_default_bareposition;
};

#endif
