/*
 * IAPBoard.cpp
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

#include <iostream>
#include <cstdio>
#include <limits>
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>
#include <boost/foreach.hpp>


#include "IAPBoard.hpp"
#include "rs232.hpp"


Axis::Axis(const boost::property_tree::ptree &pt, IAPBoard* brd) :
    axispt_(pt),
    ID_(pt.get<size_t>("<xmlattr>.id")),
    Desc_(pt.get<std::string>("<xmlattr>.descriptor")),
    UnitName_(pt.get<std::string>("UnitConversion.unitname")),
    UnitFactor_(pt.get<double>("UnitConversion.factor")),
    BaseSpeed_(pt.get<int>("BaseSpeed")),
    SlewSpeed_(pt.get<int>("SlewSpeed")),
    SlowJogSpeed_(pt.get<int>("SlowJogSpeed")),
    FastJogSpeed_(pt.get<int>("FastJogSpeed")),
    CreepSteps_(pt.get<int>("CreepSteps")),
    Accel_(pt.get<int>("Accel")),
    LowerLimit_(pt.get<int>("LowerLimit")),
    UpperLimit_(pt.get<int>("UpperLimit")),
    Position_(pt.get<int>("Position")),
    Board_(brd)
{
}

void Axis::printAxis() const
{
    using namespace std;

    cout << "Axis # " << ID_ << " " << Desc_ << endl;
    cout << "\tBaseSpeed: " << BaseSpeed_ << endl;
    cout << "\tSlewSpeed: " << SlewSpeed_ << endl;
    cout << "\tSlowJogSpeed: " << SlowJogSpeed_ << endl;
    cout << "\tFastJogSpeed: "<<FastJogSpeed_ << endl;
    cout << "\tCreepSteps: " << CreepSteps_ << endl;
    cout << "\tAccel: " << Accel_ << endl;
    cout << "\tLowerLimit: " << LowerLimit_ << endl;
    cout << "\tUpperLimit: " << UpperLimit_ << endl;
    cout << "\tPosition: " << Position_ << endl;
    cout << "\tConversion to " << UnitName_ << " Factor: " << UnitFactor_
         << endl;
}

// update the onchip axis configuration with the value stored in this
// axis object
int Axis::UpdateConfiguration() const
{
    char buf[32];

    if( Board_->setaxisnum(ID_) < 0 )
        return -1;

    sprintf(buf,"1sb%ld",BaseSpeed_); Board_->send_command_quiet(buf);
    sprintf(buf,"1sv%ld",SlewSpeed_); Board_->send_command_quiet(buf);
    sprintf(buf,"1sj%ld",SlowJogSpeed_); Board_->send_command_quiet(buf);
    sprintf(buf,"1sf%ld",FastJogSpeed_); Board_->send_command_quiet(buf);
    sprintf(buf,"1cr%ld",CreepSteps_); Board_->send_command_quiet(buf);
    sprintf(buf,"1sa%ld",Accel_); Board_->send_command_quiet(buf);
    sprintf(buf,"1ll%ld",LowerLimit_); Board_->send_command_quiet(buf);
    sprintf(buf,"1ul%ld",UpperLimit_); Board_->send_command_quiet(buf);
    sprintf(buf,"1sp%ld",Position_); Board_->send_command_quiet(buf);
    return 0;
}


void Axis::move_rel(const BarePosition::type delta) const
{
    Board_->send_command_quiet("1MR" + boost::lexical_cast<std::string>(delta));
}

void Axis::move_abs(const BarePosition::type abs) const
{
    Board_->send_command_quiet("1MA" + boost::lexical_cast<std::string>(abs));
}



IAPBoard::IAPBoard(IAPconfig& conf) :
    connected(false),
    serial_interface(STD_TR1::shared_ptr< RS232 >(new sctl_ctb(conf.GetParameters("rs232")))),
    config_(conf),
    coordinate_map(),
    axes(),
    curaxis(NULL)
{

    using boost::property_tree::ptree;
    using namespace std;

    // init default environment
    envion.axis_id = 0;

    try {
        BOOST_FOREACH(const ptree::value_type & v, config_.GetParameters()) {
            if(v.first.data() != string("axis")) continue;

            axes.push_back(Axis(v.second, this));
            //std::cout << "parsing axis "<< v.second.get_id() << "  : " << v.second.get_desc() << endl;
            std::cout << "parsing axis "<< axes.back().get_id() << "  : " << axes.back().get_desc() << endl;
            coordinate_map.insert(pair<size_t,string>(axes.back().get_id(), axes.back().get_desc()));
        }
    } catch (std::exception &e) {
        cerr << "Exception string: " << e.what() << endl;
    }

    /* opens/initializes the serial device */
    //FIXME check return value
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

    char buf[32];
    send_command("1ID", buf);
    std::cout << "FIRMWARE Version: " << buf << std::endl;

    std::cout << "set number of channels to : " << GetNrOfAxes() << std::endl;

    //TODO write GetMaxID function
    send_command_quiet("1NC" + boost::lexical_cast<std::string>(GetNrOfAxes()));

    std::cout << "initialize axes parameters" << std::endl;
    initAxes();

    send_command_quiet("1CH1"); // set axis 1
    send_command_quiet("1IR"); // disable jog mode

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



std::string IAPBoard::send_lowlevel(const std::string &cmd) const
{
    bool readflag = false;
    int len = 0;
    unsigned int idx = 0;
    size_t send_retval;
    static char buffer[1024];

    std::cout << "IAPBoard send cmd called " << cmd;
    sprintf(buffer, "%s\r\n", cmd.c_str());

    /* write() is assumed to work in one run, as the commands sent
       to the serial port have a really small size */
    send_retval = serial_interface->send(buffer, strlen(buffer));
    std::cout << " - send ret: " << send_retval;

    while(len <= 0 || readflag) {
        usleep(50000);

        //std::cout << "db1" << std::endl;
        len = serial_interface->receive(&buffer[idx], sizeof(buffer)-idx);
        //std::cout << "db2 len " << len << std::endl;
        if(len <= 0) {
            //std::cout << "db3" << std::endl;
            if(readflag) break;
        }
        else {
            //std::cout << "db4" << std::endl;
            readflag = true;
            idx += len;
        }
    }

    std::cout <<  " - received: len=" << strlen(buffer)
              << " buf: " << buffer;

    return std::string(buffer);
}

// sends cmd to stepper board and return reply string if return value
// is 0 otherwise just return error code
int IAPBoard::send_command(const std::string & cmd, char* replymsg) const
{
    std::string reply = send_lowlevel(cmd);

    // analyze reply msg from controller
    if(reply[2] == '!') {
        serial_interface->rslog(reply,"CONTROLLER ERROR ");
        return -E_PM381_ERROR_MSG;
    }

    // if(buffer[0] == ' ') {
    //     cout << "command with special return value : " << cmd;
    //     return;
    // }

    if(reply.length() > 5) {
        if(replymsg) {
            std::string tmp = reply.substr(3,reply.length()-5);
            strcpy(replymsg,tmp.c_str());
        }
        return 0;
    }
    else
        return -E_SIZE_PM381_REPLY_SHORT;
}

/* returns the integer return code of the command */
/* the return code of this function should only be used for commands
   which return an interger value */

// TODO remove this function and use a default value for reply in send_command
int IAPBoard::send_command_quiet(const std::string & cmd) const
{
    return send_command(cmd, NULL);
}

int IAPBoard::send_getint_command(const std::string & cmd) const
{
    std::cout << "send getint cmd called " <<  cmd << std::endl;

    std::string reply = send_lowlevel(cmd);

    if(reply[2] == '!'){
        serial_interface->rslog(reply,"CONTROLLER ERROR : ");
        return std::numeric_limits<int>::min();
    }

    if(reply.length() > 5)
        return boost::lexical_cast<int>(reply.substr(3,reply.length()-5));
    else
        return std::numeric_limits<int>::min();
}


BarePosition IAPBoard::createBarePosition(void) const
{
    static BarePosition iap_default_bareposition(coordinate_map);
    return iap_default_bareposition;
}

Position IAPBoard::createPosition(void) const
{
    static Position iap_default_position(coordinate_map);
    return iap_default_position;
}


void IAPBoard::conv2bareposition(BarePosition& ret, const Position &pos) const
{
    for(const_axesiter it = axes.begin(); it != axes.end(); ++it)
        ret.SetCoordinate(it->get_id(),
                          static_cast<BarePosition::type>(
                              pos.GetCoordinate(it->get_id())/
                              it->get_factor()));
}

void IAPBoard::conv2postion(Position& ret, const BarePosition &bpos) const
{
    for(const_axesiter it = axes.begin(); it != axes.end(); ++it)
        ret.SetCoordinate(it->get_id(),
                          static_cast<Position::type>(
                              bpos.GetCoordinate(it->get_id())*
                              it->get_factor()));
}


void IAPBoard::SaveEnvironment()
{
    if(!curaxis) return;

    // store current axis
    envion.axis_id = curaxis->get_id();
}

void IAPBoard::RestoreEnvironment()
{
    //restore saved axis
    setaxisnum(envion.axis_id);
}

int IAPBoard::initAxes()
{
    for(const_axesiter it = axes.begin(); it != axes.end(); ++it)
    {
        it->UpdateConfiguration();
        it->printAxis();
    }
    return 0;
}

Axis* IAPBoard::getAxis(const size_t id)
{
    axesiter it = find_if(axes.begin(),axes.end(),
                          [=](Axis& ax) { return( ax.get_id() == id); });
    return (it == axes.end()) ? NULL : &(*it);
}

Axis* IAPBoard::getAxis(const std::string desc)
{
    axesiter it = find_if(axes.begin(),axes.end(),
                          [=](Axis& ax) { return( ax.get_desc() == desc); });
    return (it == axes.end()) ? NULL : &(*it);
}

int IAPBoard::setaxisnum(const size_t id)
{
    int ret = 0;

    Axis* aptr = getAxis(id);
    if(!aptr) {
        std::cout << id << " is not a valid axis id" << std::endl;
        ret = -E_AXIS_NUM_INVALID;
    }

    ret = send_command_quiet("1CH" + boost::lexical_cast<std::string>(id));
    if(ret >= 0)
        curaxis = aptr;

    return ret;
}


void IAPBoard::move_rel(const Position& rel)
{
    BarePosition bp = createBarePosition();
    conv2bareposition(bp, rel);
    move_rel(bp);
}

//TODO implement/check SOFT_LIMITS
void IAPBoard::move_rel(const BarePosition& rel)
{
    SaveEnvironment();

    for(axesiter it = axes.begin(); it != axes.end(); ++it)
    {
        setaxisnum(it->get_id());
        it->move_rel(rel.GetCoordinate(it->get_id()));
    }

    RestoreEnvironment();
}

void IAPBoard::move_to(const Position& abs)
{
    BarePosition bp = createBarePosition();
    conv2bareposition(bp, abs);
    move_to(bp);
}

//TODO implement/check SOFT_LIMITS
void IAPBoard::move_to(const BarePosition& abs)
{
    SaveEnvironment();

    for(axesiter it = axes.begin(); it != axes.end(); ++it)
    {
        setaxisnum(it->get_id());
        it->move_abs(abs.GetCoordinate(it->get_id()));
    }

    RestoreEnvironment();
}


void IAPBoard::get_cur_position(BarePosition& retbarepos) const
{
    //only for PM381
    char buf[128];
    send_command("1QP",buf);
    const std::string line(buf);

    //FIXME parse id!
    const boost::regex re("= [-]?\\d+");

    // for(size_t i=0; i < line.length(); ++i)
    // {
    //     printf("%d:%02x ",i,(unsigned char)line[i]);
    //     if(line[i] == '\n' || line[i] == '\r')
    //         line[i] = ' ';
    // }

    boost::sregex_token_iterator tokens(line.begin(),line.end(),re);
    boost::sregex_token_iterator end;

    for (size_t id=1; id <= GetNrOfAxes(); id++, ++tokens){
        if(tokens == end) {
            std::cerr << "WARNING: parsing in get_cur_position failed" << std::endl;
            return;
        }

        std::cout << "gcp tokens: " << *tokens << std::endl;
        std::stringstream tmp(tokens->str().substr(1));
        BarePosition::type val;
        tmp >> val;
        retbarepos.SetCoordinate(id,val);
    }
}

void IAPBoard::get_cur_position(Position& retpos) const
{
    BarePosition bp = createBarePosition();
    get_cur_position(bp);
    conv2postion(retpos, bp);
}


void IAPBoard::test(char *msg)
{
    //FIXME reimplement test function

    //char buffer[1024];

    // //waits for incoming dota, if no data arrives blocks forever
    // serial_interface->rslog(" serial test", "#>");

    // while(serial_interface->receive(buffer, 1024) <= 0 ) {
    //     sleep(1);
    // }
    // sock.async_send(buffer);
}

void IAPBoard::reset()
{
    std::cout << "board: reset" << std::endl;
}


//Send Command 1SP0 to each axis
int IAPBoard::SetZero()
{
    std::cout << "board: set current positon to zero" << std::endl;
    SaveEnvironment();

    for(axesiter it = axes.begin(); it != axes.end(); ++it)
    {
        int ret = setaxisnum(it->get_id());
        if (ret < 0){
            RestoreEnvironment();
            return ret;
        }

        send_command_quiet("1SP0");
    }

    RestoreEnvironment();
    return 0;
}
