#include <iostream>
#include <cstdio>

#include "IAPBoard.hpp"

using namespace std;

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
void IAPBoard::connect(session& sock)
{
     std::cout << "board: connecting to sctlbrd" << std::endl;

     /* connection commands */
     /* TODO */

     initAxis(sock);
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


/* returns the integer return code of the command */
int IAPBoard::send_command_quiet(const std::string & cmd)
{
    char buffer[1024];
    
    std::cout << "send cmd (quiet) called " <<  cmd << std::endl;

    std::string tosend = cmd + "\r\n"; /* maybe not needed */
    while(serial_interface->send(tosend.c_str(), tosend.length()) <= 0 ) {
        sleep(1);
    }
    while(serial_interface->receive(buffer, sizeof(buffer)) <= 0 ) {
        sleep(1);
    }

    if(buffer[2] == '!'){
        serial_interface->rslog(buffer,"CONTROLLER ERROR : ");
        //FIXME error code
        return -1;
    }

    if(strlen(buffer) > 2) {
        buffer[strlen(buffer)-2] = '\0';
    }
    return atoi(&buffer[3]);
}


void IAPBoard::send_command(session &sock, const std::string & cmd)
{
    char buffer[1024];
    
    cout << "send cmd called " <<  cmd << endl;

    string tosend = cmd + "\r\n"; /* maybe not needed */
    while(serial_interface->send(tosend.c_str(), tosend.length()) <= 0 ) {
        sleep(1);
    }
    while(serial_interface->receive(buffer, sizeof(buffer)) <= 0 ) {
        sleep(1);
    }

    cout << buffer[0] << " " << buffer[1] << " " << strlen(buffer) << " : " << buffer << endl;
    if(buffer[2] == '!'){
        sock.async_send("CONTROLLER ERROR"); // FIXME also send buffer;
        serial_interface->rslog(buffer,"CONTROLLER ERROR : ");
        return;
    }

    // if(buffer[0] == ' ') {
    //     cout << "command with special return value : " << cmd;
    //     return;
    // }

    if(strlen(buffer) > 2) {
        buffer[strlen(buffer)-2] = '\0';
        sock.async_send(&buffer[3]); // FIXME also send buffer;
    }
    else
        sock.async_send("size of reply from stepper card was too short");
}

int IAPBoard::getaxisnum() const
{
    return (curaxis-axis)/sizeof(struct mct_Axis);
}

static string binary( unsigned long n )
{
    char     result[ 4 + 1 ];
    unsigned index  = 4;
    result[ index ] = '\0';

    do {
        result[ --index ] = '0' + (n & 1);
        n >>= 1;
    } while (index);

    return string(result);
}

int IAPBoard::initAxis(session& sock)
{
    int error;
    unsigned int i;
    struct mct_Axis *curaxis;
    char axstr[4], buf[80];
    
    error = 0;
    
    for(i=1; i <= NR_AXIS; ++i){
        snprintf(axstr,4, "<%d>",i);
        curaxis = &axis[i-1];
        
        curaxis->axis_BaseSpeed = axisconfig.get_int_param("BaseSpeed"+string(axstr));
        curaxis->axis_SlewSpeed = axisconfig.get_int_param("SlewSpeed"+string(axstr));
        curaxis->axis_SlowJogSpeed = axisconfig.get_int_param("SlowJogSpeed"+string(axstr));
        curaxis->axis_FastJogSpeed = axisconfig.get_int_param("FastJogSpeed"+string(axstr));
        curaxis->axis_CreepSteps = axisconfig.get_int_param("CreepSteps"+string(axstr));
        curaxis->axis_Accel = axisconfig.get_int_param("Accel"+string(axstr));
        curaxis->axis_LowerLimit = axisconfig.get_int_param("LowerLimit"+string(axstr));
        curaxis->axis_UpperLimit = axisconfig.get_int_param("UpperLimit"+string(axstr));
        curaxis->axis_Position = axisconfig.get_int_param("Position"+string(axstr));
        curaxis->axis_MotorStatus = axisconfig.get_int_param("MotorStatus"+string(axstr));
        curaxis->axis_Multiplier = axisconfig.get_double_param("Multiplier"+string(axstr));

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

        setaxisnum(sock,i-1,true);

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

int IAPBoard::setaxisnum(session& sock, const unsigned int axisnum, bool quiet=false)
{
    unsigned int curaxisnum  = (curaxis-axis)/sizeof(struct mct_Axis);

    cout << "curaxis " << curaxisnum << " axisnum " << axisnum << endl;
    
    //if (axisnum == curaxisnum) return - 1;

    if(axisnum >= NR_AXIS) {
        cout << " axnium "  << axisnum << " is not a valid axinum" << endl;
        if(!quiet)
            sock.async_send("INVALID AXIS NUMBER");
            
        return -EINVAL;
    }

    if(quiet)
        send_command_quiet("1WP"+binary(axisnum));
    else
        send_command(sock, "1WP"+binary(axisnum));

    curaxis = &axis[axisnum];
    return curaxisnum;
}

void IAPBoard::test(session &sock)
{
    char buffer[1024];

    //waits for incoming dota, if no data arrives blocks forever
    serial_interface->rslog(" serial test", "#>");

    while(serial_interface->receive(buffer, 1024) <= 0 ) {
        sleep(1);
    }
    sock.async_send(buffer);
}

void IAPBoard::reset()
{
 std::cout << "board: reset" << std::endl;
}
