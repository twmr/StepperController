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


IAPBoard::IAPBoard(IAPconfig& conf) :
    connected(false),
    serial_interface(STD_TR1::shared_ptr< RS232 >(new sctl_ctb(conf.GetParameters("rs232")))),
    convconsts(STD_TR1::shared_ptr< ConversionConstants >(new ConversionConstants())),
    config_(conf),
    curaxis(&axis[0])
{
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

int IAPBoard::getaxisnum() const
{
    return curaxis-axis + 1;
}

int IAPBoard::initAxes()
{
    using boost::property_tree::ptree;
    using namespace std;

    char buf[80];

    try {
        BOOST_FOREACH(const ptree::value_type & v, config_.GetParameters()) {
            if(v.first.data() != string("axis")) continue;

            size_t id = v.second.get<size_t>("<xmlattr>.id");
            string desc = v.second.get<string>("<xmlattr>.descriptor");

            if( setaxisnum(id) < 0 )
                return -1;

            curaxis->axis_BaseSpeed = v.second.get<int>("BaseSpeed");
            curaxis->axis_SlewSpeed = v.second.get<int>("SlewSpeed");
            curaxis->axis_SlowJogSpeed = v.second.get<int>("SlowJogSpeed");
            curaxis->axis_FastJogSpeed = v.second.get<int>("FastJogSpeed");
            curaxis->axis_CreepSteps = v.second.get<int>("CreepSteps");
            curaxis->axis_Accel = v.second.get<int>("Accel");
            curaxis->axis_LowerLimit = v.second.get<int>("LowerLimit");
            curaxis->axis_UpperLimit = v.second.get<int>("UpperLimit");
            curaxis->axis_Position = v.second.get<int>("Position");
            curaxis->axis_MotorStatus = v.second.get<int>("MotorStatus");
            curaxis->axis_Multiplier = v.second.get<double>("Multiplier");

            cout << "Axis # " << id << " " << desc << endl;
            cout << "\tBaseSpeed: " << curaxis->axis_BaseSpeed << endl;
            cout << "\tSlewSpeed: " << curaxis->axis_SlewSpeed << endl;
            cout << "\tSlowJogSpeed: " << curaxis->axis_SlowJogSpeed << endl;
            cout << "\tFastJogSpeed: "<<curaxis->axis_FastJogSpeed << endl;
            cout << "\tCreepSteps: " << curaxis->axis_CreepSteps << endl;
            cout << "\tAccel: " << curaxis->axis_Accel << endl;
            cout << "\tLowerLimit: " << curaxis->axis_LowerLimit << endl;
            cout << "\tUpperLimit: " << curaxis->axis_UpperLimit << endl;
            cout << "\tPosition: " << curaxis->axis_Position << endl;
            cout << "\tMotorStatus: " << (long int)curaxis->axis_MotorStatus << endl;
            cout << "\tMultiplier: " << curaxis->axis_Multiplier << endl;

            sprintf(buf,"1SB%ld",curaxis->axis_BaseSpeed);
            send_command_quiet(buf);

            sprintf(buf,"1SV%ld",curaxis->axis_SlewSpeed);
            send_command_quiet(buf);

            sprintf(buf,"1SJ%ld",curaxis->axis_SlowJogSpeed);
            send_command_quiet(buf);

            sprintf(buf,"1SF%ld",curaxis->axis_FastJogSpeed);
            send_command_quiet(buf);

            sprintf(buf,"1CR%ld",curaxis->axis_CreepSteps);
            send_command_quiet(buf);

            sprintf(buf,"1SA%ld",curaxis->axis_Accel);
            send_command_quiet(buf);

            sprintf(buf,"1LL%ld",curaxis->axis_LowerLimit);
            send_command_quiet(buf);

            sprintf(buf,"1UL%ld",curaxis->axis_UpperLimit);
            send_command_quiet(buf);

            sprintf(buf,"1SP%ld",curaxis->axis_Position);
            send_command_quiet(buf);

            //FIXME check every output of the previous send_commands
        }
    } catch (...)
    {
        std::cerr << "Exception thrown" << std::endl;
    }

    return 0;
}

int IAPBoard::setaxisnum(const unsigned int axisnum)
{
    int ret = 0;

    std::cout << "old axis " << getaxisnum() << " axisnum " << axisnum << std::endl;

    if(axisnum == 0 || axisnum > GetNrOfAxes()) {
        std::cout << axisnum << " is not a valid axis number" << std::endl;
        return -E_AXIS_NUM_INVALID;
    }

    ret = send_command_quiet("1CH" + boost::lexical_cast<std::string>(axisnum));
    if(ret >= 0)
        curaxis = &axis[axisnum-1];
    return ret;
}


void IAPBoard::move_rel(const Position& rel)
{
    BarePosition bp;
    convconsts.get()->get_bare_position(bp, rel);
    move_rel(bp);
}

//TODO implement/check SOFT_LIMITS
void IAPBoard::move_rel(const BarePosition& rel)
{
    const int prevaxis = getaxisnum();
    setaxisnum(1);
    send_command_quiet("1MR" + boost::lexical_cast<std::string>(rel.get_x()));
    setaxisnum(2);
    send_command_quiet("1MR" + boost::lexical_cast<std::string>(rel.get_y()));
    setaxisnum(3);
    send_command_quiet("1MR" + boost::lexical_cast<std::string>(rel.get_theta()));
    setaxisnum(prevaxis);
}

void IAPBoard::move_to(const Position& abs)
{
    BarePosition bp;
    convconsts.get()->get_bare_position(bp, abs);
    move_to(bp);
}

//TODO implement/check SOFT_LIMITS
void IAPBoard::move_to(const BarePosition& abs)
{
    const int prevaxis = getaxisnum();
    setaxisnum(1);
    send_command_quiet("1MA" + boost::lexical_cast<std::string>(abs.get_x()));
    setaxisnum(2);
    send_command_quiet("1MA" + boost::lexical_cast<std::string>(abs.get_y()));
    setaxisnum(3);
    send_command_quiet("1MA" + boost::lexical_cast<std::string>(abs.get_theta()));
    setaxisnum(prevaxis);
}


void IAPBoard::get_cur_position(BarePosition& retbarepos) const
{
    //only for PM381
    char buf[128];
    send_command("1QP",buf);
    const std::string line(buf);
    std::vector<BarePosition::type> vals(GetNrOfAxes());
    const boost::regex re("= [-]?\\d+");

    // for(size_t i=0; i < line.length(); ++i)
    // {
    //     printf("%d:%02x ",i,(unsigned char)line[i]);
    //     if(line[i] == '\n' || line[i] == '\r')
    //         line[i] = ' ';
    // }

    boost::sregex_token_iterator tokens(line.begin(),line.end(),re);
    boost::sregex_token_iterator end;

    for (size_t i=0; i < 3; i++, ++tokens){
        if(tokens == end) {
            std::cerr << "WARNING: parsing in get_cur_position failed" << std::endl;
            return;
        }

        std::cout << "gcp tokens: " << *tokens << std::endl;
        std::stringstream tmp(tokens->str().substr(1));
        tmp >> vals[i];
        // vals[i] = boost::lexical_cast<BarePosition::type>(tokens->str().substr(1));
    }

    retbarepos.SetPosition(vals[0],vals[1],vals[2]);
}

void IAPBoard::get_cur_position(Position& retpos) const
{
    BarePosition bp;
    get_cur_position(bp);
    convconsts.get()->get_position(retpos, bp);
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

int IAPBoard::SetZero()
{
    std::cout << "board: set current positon to zero" << std::endl;
    const size_t old_axis = getaxisnum();

    for(size_t i = 1; i <= GetNrOfAxes(); i++) {
        int ret = setaxisnum(i);
        if (ret < 0)
            return ret;
        send_command_quiet("1SP0");
    }
    int ret = setaxisnum(old_axis);
    if (ret < 0)
        return ret;
    else
        return 0;
}
