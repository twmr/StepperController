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

#include "IAPBoard.hpp"

IAPBoard::IAPBoard(const RS232config & serialconf, const IAPconfig & axisconf) :
    connected(false),
    serial_interface(STD_TR1::shared_ptr< RS232 >(new sctl_ctb(serialconf))),
    axisconfig(axisconf),
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

    initAxis();
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



/* cmd to be sent is stored in buffer. return string is also
   returned in buffer */
void IAPBoard::send_lowlevel(char * buffer, const size_t size)
{
    bool readflag = false;
    int len = 0;
    unsigned int idx = 0;

    /* write() is assumed to work in one run, as the commands sent
       to the serial port have a really small size */
    serial_interface->send(buffer, strlen(buffer));

    while(len <= 0 || readflag) {
        usleep(50000);

        //cout << "db1" << endl;
        len = serial_interface->receive(&buffer[idx], size-idx);
        //cout << "db2 len " << len << endl;
        if(len <= 0) {
            //cout << "db3" << endl;
            if(readflag) break;
        }
        else {
            //cout << "db4" << endl;
            readflag = true;
            idx += len;
        }
    }

    std::cout << buffer[0] << " " << buffer[1] << " " << strlen(buffer)
              << " : " << buffer << std::endl;

}

int IAPBoard::send_command(const std::string & cmd, char* reply)
{
    static char buffer[1024];

    std::cout << "send cmd called " <<  cmd << std::endl;
    sprintf(buffer, "%s\r\n", cmd.c_str());

    send_lowlevel(buffer, sizeof(buffer));

    if(buffer[2] == '!') {
        serial_interface->rslog(buffer,"CONTROLLER ERROR ");
        return -E_PM301_ERROR_MSG;
    }

    // if(buffer[0] == ' ') {
    //     cout << "command with special return value : " << cmd;
    //     return;
    // }

    if(strlen(buffer) > 2 && reply) {
        buffer[strlen(buffer)-2] = '\0';
        strcpy(reply,&buffer[3]);
        return 0;
    }
    else
        return -E_SIZE_PM301_REPLY_SHORT;
}

/* returns the integer return code of the command */
/* the return code of this function should only be used for commands
   which return an interger value */

int IAPBoard::send_command_quiet(const std::string & cmd) {
    return send_command(cmd, NULL);
}

int IAPBoard::send_getint_command(const std::string & cmd) {
    static char buffer[1024];

    std::cout << "send getint cmd called " <<  cmd << std::endl;
    sprintf(buffer, "%s\r\n", cmd.c_str());

    send_lowlevel(buffer, sizeof(buffer));

    if(buffer[2] == '!'){
        serial_interface->rslog(buffer,"CONTROLLER ERROR : ");
        return std::numeric_limits<int>::min();
    }

    if(strlen(buffer) > 2) {
        buffer[strlen(buffer)-2] = '\0';
        return atoi(&buffer[3]);
    }
    else
        return std::numeric_limits<int>::min();
}

int IAPBoard::getaxisnum() const
{
    return (curaxis-axis)/sizeof(struct mct_Axis);
}

// static std::string binary( unsigned long n )
// {
//     char     result[ 4 + 1 ];
//     unsigned index  = 4;
//     result[ index ] = '\0';

//     do {
//         result[ --index ] = '0' + (n & 1);
//         n >>= 1;
//     } while (index);

//     return std::string(result);
// }

int IAPBoard::initAxis()
{
    int error;
    unsigned int i;
    struct mct_Axis *curaxis;
    char axstr_[4], buf[80];
    std::string axstr;;
    
    error = 0;
    
    for(i=1; i <= NR_AXIS; ++i){
        snprintf(axstr_,4, "<%d>",i);
        curaxis = &axis[i-1];
        axstr = axstr_;
        
        curaxis->axis_BaseSpeed = axisconfig.get_int_param("BaseSpeed"+axstr);
        curaxis->axis_SlewSpeed = axisconfig.get_int_param("SlewSpeed"+axstr);
        curaxis->axis_SlowJogSpeed = axisconfig.get_int_param("SlowJogSpeed"+axstr);
        curaxis->axis_FastJogSpeed = axisconfig.get_int_param("FastJogSpeed"+axstr);
        curaxis->axis_CreepSteps = axisconfig.get_int_param("CreepSteps"+axstr);
        curaxis->axis_Accel = axisconfig.get_int_param("Accel"+axstr);
        curaxis->axis_LowerLimit = axisconfig.get_int_param("LowerLimit"+axstr);
        curaxis->axis_UpperLimit = axisconfig.get_int_param("UpperLimit"+axstr);
        curaxis->axis_Position = axisconfig.get_int_param("Position"+axstr);
        curaxis->axis_MotorStatus = axisconfig.get_int_param("MotorStatus"+axstr);
        curaxis->axis_Multiplier = axisconfig.get_double_param("Multiplier"+axstr);

        printf("Axis # %d\n",i);
        printf("BaseSpeed: %ld\n",curaxis->axis_BaseSpeed);
        printf("SlewSpeed: %ld\n",curaxis->axis_SlewSpeed);
        printf("SlowJogSpeed: %ld\n",curaxis->axis_SlowJogSpeed);
        printf("FastJogSpeed: %ld\n",curaxis->axis_FastJogSpeed);
        printf("CreepSteps: %ld\n",curaxis->axis_CreepSteps);
        printf("Accel: %ld\n",curaxis->axis_Accel);
        printf("LowerLimit: %ld\n",curaxis->axis_LowerLimit);
        printf("UpperLimit: %ld\n",curaxis->axis_UpperLimit);
        printf("Position: %ld\n",curaxis->axis_Position);
        printf("MotorStatus: %ld\n",(long int)curaxis->axis_MotorStatus);
        printf("Multiplier: %3.2f\n",curaxis->axis_Multiplier);
        printf("\n");

        setaxisnum(i-1);

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
    }

    //FIXME check every output of the previous send_commands
    
    return 0;
}

int IAPBoard::setaxisnum(const unsigned int axisnum)
{
    unsigned int curaxisnum  = (curaxis-axis)/sizeof(struct mct_Axis);

    std::cout << "curaxis " << curaxisnum << " axisnum " << axisnum << std::endl;
    
    //if (axisnum == curaxisnum) return - 1;

    if(axisnum >= NR_AXIS) {
        std::cout << " axnium "  << axisnum << " is not a valid axinum" << std::endl;

        //FIXME move this to server
        //if(!quiet)
        //    sock.async_send("INVALID AXIS NUMBER");
            
        return -EINVAL;
    }

    //FIXME move verbose send to server
    //if(quiet)
    send_command_quiet("1CH" + boost::lexical_cast<std::string>(axisnum+1));
    //else
    //    send_command(sock, "1WP"binary(axisnum));

    curaxis = &axis[axisnum];
    return curaxisnum;
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
