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

//TODO use refereneces or smart pointers
Axis::Axis(const size_t id, const std::string desc, IAPBoard* brd) :
    ID_(id),
    Desc_(desc),
    Board_(brd)
{
    const boost::property_tree::ptree & pt = Board_->getConfig().getaxispt(ID_);
    parse_vars(pt);
}

void Axis::parse_vars(const boost::property_tree::ptree & pt)
{
    BaseSpeed_ = pt.get<int>("BaseSpeed");
    SlewSpeed_ = pt.get<int>("SlewSpeed");
    SlowJogSpeed_ = pt.get<int>("SlowJogSpeed");
    FastJogSpeed_ = pt.get<int>("FastJogSpeed");
    CreepSteps_ = pt.get<int>("CreepSteps");
    Accel_ = pt.get<int>("Accel");

    LowerLimit_ = pt.get<BarePosition::type>("LowerLimit");
    UpperLimit_ = pt.get<BarePosition::type>("UpperLimit");
    Position_ = pt.get<BarePosition::type>("Position");

    UnitName_ = pt.get<std::string>("UnitConversion.unitname");
    UnitFactor_ = pt.get<double>("UnitConversion.factor");
    Offset_ = pt.get<BarePosition::type>("UnitConversion.offset");
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
    cout << "\tOffset: " << Offset_ << endl;
    cout << "\tConversion to " << UnitName_ << " Factor: " << UnitFactor_
         << endl;
}

// update the onchip axis configuration with the value stored in this
// axis object
int Axis::UpdateConfiguration()
{
    char buf[32];

    if( Board_->setaxisnum(ID_) < 0 )
        return -1;

    // TODO try catch block ?!?!?
    const boost::property_tree::ptree & pt = Board_->getConfig().getaxispt(ID_);
    parse_vars(pt);

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


int Axis::move_rel(const BarePosition::type delta) const
{
  return Board_->send_command_quiet("1MR" + 
				    boost::lexical_cast<std::string>(delta));
}

int Axis::move_abs(const BarePosition::type abs) const
{
  return Board_->send_command_quiet("1MA" + 
				    boost::lexical_cast<std::string>(abs));
}



IAPBoard::IAPBoard(IAPconfig& conf, const bool initialize=false) :
    init_(initialize),
    connected_(false),
    serial_interface(STD_TR1::shared_ptr< RS232 >(new sctl_ctb(conf.GetParameters("rs232")))),
    config_(conf),
    coordinate_map(),
    axes(),
    curaxis(NULL),
    iap_default_position(),
    iap_default_bareposition()
{

    using boost::property_tree::ptree;
    using namespace std;

    // init default environment
    envion.axis_id = 0;

    try {
        BOOST_FOREACH(const ptree::value_type & v, config_.GetParameters()) {
            if(v.first.data() != string("axis"))
                continue;

            size_t id = v.second.get<size_t>("<xmlattr>.id");
            std::string desc = v.second.get<std::string>("<xmlattr>.descriptor");
            axes.push_back(Axis(id, desc, this));
            std::cout << "parsing axis "<< axes.back().get_id() << "  : " << axes.back().get_desc() << endl;

            //creating bidirectional maps for axis descriptor <-> axis id conversion
            coordinate_map.insert(pair<size_t,string>(axes.back().get_id(), axes.back().get_desc()));
            unit_map.insert(pair<size_t,string>(axes.back().get_id(), axes.back().get_unitname()));
            inv_coordinate_map.insert(pair<string,size_t>(axes.back().get_desc(), axes.back().get_id()));

            iap_default_position.SetCoordinate(axes.back().get_id(), 0);
            iap_default_bareposition.SetCoordinate(axes.back().get_id(), 0);
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
    if(is_connected())
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

    //get the max axis ID
    std::vector<Axis>::iterator itor =
        std::max_element(axes.begin(), axes.end(),
                         [](Axis &i1, Axis &i2) {
                             return i1.get_id() < i2.get_id(); });

    send_command_quiet("1NC" + boost::lexical_cast<std::string>(itor->get_id()));

    std::cout << "initialize axes parameters" << std::endl;
    initAxes();

    send_command_quiet("1CH1"); // set axis 1
    send_command_quiet("1IR"); // disable jog mode

    connected_ = true;
}


//shut down the Steppercontrolboard to prevent overheating of the steppermotors
void IAPBoard::disconnect()
{
    std::cout << "board: disconnecting from sctlbrd" << std::endl;

    /* disconnection commands */
    /* TODO */

    connected_ = false;
}

//removes junk characters from messages which are sent back from the PM381 controller
//there are 3 types of messages:
// 01:!  error-string\r\n
// 01:OK\r\n
// 01:string\r\n
std::string IAPBoard::strip_ctrl_junk(const std::string &rply) const
{
    const size_t errpos = 3; //index of the ! in the error messages of the PM381
    size_t start = errpos;
    if(rply[errpos] == '!')
        start = errpos + 1;

    std::string reptrunc = rply.substr(0,rply.length()-2);
    // std::cout << "strip last2chars: |" << reptrunc << "|" << std::endl;

    reptrunc = reptrunc.substr(start);
    size_t pos = reptrunc.find_first_not_of(" ");
    reptrunc = reptrunc.substr(pos);
    // std::cout << "cleaned string: |" << reptrunc << "|" << std::endl;

    return reptrunc;
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

#ifdef SERIAL_DEBUG // if defined run without hardware
    if(cmd == "ERRORMSG_TEST")
        sprintf(buffer,"01:! this is a sample error msg\r\n");
#endif

    std::cout <<  " - received: len=" << strlen(buffer)
              << " buf: " << buffer;

    return std::string(buffer);
}

// sends cmd to stepper board and return reply string if return value
// is 0 otherwise just return error code
int IAPBoard::send_command(const std::string & cmd, char* replymsg) const
{
    std::string reply = send_lowlevel(cmd);
    if(reply.length() < 5)
            return -E_SIZE_PM381_REPLY_SHORT;

    std::string reptrunc = strip_ctrl_junk(reply);

    if(replymsg) strcpy(replymsg,reptrunc.c_str());

    // analyze reply msg from controller
    if(reply[3] == '!') {
        serial_interface->rslog(reptrunc ,"CONTROLLER ERROR ");
        //FIXME locking ?!?!
        latest_error_str_ = reptrunc;
        return -E_PM381_ERROR_MSG;
    }
    return 0;
}


// TODO remove this function and use a default value for reply in send_command
int IAPBoard::send_command_quiet(const std::string & cmd) const
{
    return send_command(cmd, NULL);
}

/* returns the integer return code of the command */
/* the return code of this function should only be used for commands
   which return an interger value */
int IAPBoard::send_getint_command(const std::string & cmd) const
{
    // returning minval means an error has happened
    int minval = std::numeric_limits<int>::min();

    std::cout << "send getint cmd called " <<  cmd << std::endl;

    std::string reply = send_lowlevel(cmd);
    if(reply.length() < 5)
            return minval;

    std::string reptrunc = strip_ctrl_junk(reply);
    if(reply[3] == '!'){
        serial_interface->rslog(reptrunc,"CONTROLLER ERROR : ");
        //FIXME locking ?!?!
        latest_error_str_ = reptrunc;
        return minval;
    }

    return boost::lexical_cast<int>(reptrunc);
}


BarePosition IAPBoard::createBarePosition(void) const
{
    return iap_default_bareposition;
}

Position IAPBoard::createPosition(void) const
{
    return iap_default_position;
}


void IAPBoard::conv2bareposition(BarePosition& ret, const Position &pos) const
{
    for(Position::coord_type::const_iterator
            it = pos.begin(); it != pos.end(); ++it) {

        //check that axis is registered, otherwise we can't obtain the
        //conversionfactor
        if(!getAxis(it->first)) return;

        ret.SetCoordinate(it->first,
                          static_cast<BarePosition::type>(
                              pos.GetCoordinate(it->first)/
                              getAxis(it->first)->get_factor() + getAxis(it->first)->GetOffset()));
    }
}

void IAPBoard::conv2postion(Position& ret, const BarePosition &bpos) const
{
    // for(const_axesiter it = axes.begin(); it != axes.end(); ++it)
    for(BarePosition::coord_type::const_iterator
            it = bpos.begin(); it != bpos.end(); ++it) {

        //check that axis is registered, otherwise we can't obtain the
        //conversionfactor
        if(!getAxis(it->first)) return;

        //conversion factor: user-specified-unit/steps
        ret.SetCoordinate(it->first,
                          static_cast<Position::type>(
                              (bpos.GetCoordinate(it->first) - getAxis(it->first)->GetOffset()) *
                              getAxis(it->first)->get_factor()));
    }

}


void IAPBoard::SaveEnvironment() const
{
    if(!curaxis) return;

    // store current axis
    envion.axis_id = curaxis->get_id();
}

void IAPBoard::RestoreEnvironment() const
{
    //restore saved axis
    save_setaxisnum(envion.axis_id);
}

int IAPBoard::initAxes()
{
    for(axesiter it = axes.begin(); it != axes.end(); ++it)
    {
        if(init_) {
            config_.setAxisElement<BarePosition::type>(it->get_id(),"Position", 0);
            config_.setAxisElement<BarePosition::type>(it->get_id(),"UnitConversion.offset", 0);
            size_t halfturns = config_.getAxisElement<size_t>(it->get_id(),"init.halfaxisturns");
            double rangefac = config_.getAxisElement<double>(it->get_id(),"init.prerangefac");
            // double unitfac = getConfig().getAxisElement<BarePosition::type>(it->get_id(),"UnitConversion.factor");
            BarePosition::type newupperSL = static_cast<BarePosition::type>(floor((rangefac/2.0)*halfturns*200));
            BarePosition::type newlowerSL = -newupperSL;
            std::cout << "lowerSL: " << newlowerSL << " is [halfturns] " << newlowerSL/200.0
                      << "; upperSL: " << newupperSL << " is [halfturns]: " << newupperSL/200.0 << std::endl;
            config_.setAxisElement<BarePosition::type>(it->get_id(),"LowerLimit", newlowerSL);
            config_.setAxisElement<BarePosition::type>(it->get_id(),"UpperLimit", newupperSL);
        }
        it->UpdateConfiguration();
        it->printAxis();
    }
    return 0;
}

// Axis* IAPBoard::getAxis(const size_t id)
// {
//     axesiter it = find_if(axes.begin(),axes.end(),
//                           [=](Axis& ax) { return( ax.get_id() == id); });
//     return (it == axes.end()) ? NULL : &(*it);
// }

// Axis* IAPBoard::getAxis(const std::string desc)
// {
//     axesiter it = find_if(axes.begin(),axes.end(),
//                           [=](Axis& ax) { return( ax.get_desc() == desc); });
//     return (it == axes.end()) ? NULL : &(*it);
// }

const Axis * IAPBoard::getAxis(const size_t id) const
{

    const_axesiter it = find_if(axes.begin(),axes.end(),
                          [=](const Axis& ax) { return( ax.get_id() == id); });
    return (it == axes.end()) ? NULL : &(*it);
}

const Axis * IAPBoard::getAxis(const std::string desc) const
{
    const_axesiter it = find_if(axes.begin(),axes.end(),
                          [=](const Axis& ax) { return( ax.get_desc() == desc); });
    return (it == axes.end()) ? NULL : &(*it);
}

int IAPBoard::setaxisnum(const size_t id)
{
    int ret = 0;

    Axis const * const aptr = getAxis(id);
    if(!aptr) {
        std::cout << id << " is not a valid axis id" << std::endl;
        ret = -E_AXIS_NUM_INVALID;
    }

    ret = send_command_quiet("1CH" + boost::lexical_cast<std::string>(id));
    if(ret >= 0)
        curaxis = const_cast<Axis *>(aptr);

    return ret;
}

//this function is intended to be called only within a SAVE/RESTORE environment block
int IAPBoard::save_setaxisnum(const size_t id) const
{
    int ret = 0;

    Axis const * const aptr = getAxis(id);
    if(!aptr) {
        std::cout << id << " is not a valid axis id" << std::endl;
        ret = -E_AXIS_NUM_INVALID;
    }

    ret = send_command_quiet("1CH" + boost::lexical_cast<std::string>(id));
    if(ret >= 0)
        save_curaxis = const_cast<Axis *>(aptr);

    return ret;
}

int IAPBoard::move_rel(const Position& rel) const
{
    BarePosition bp;
    conv2bareposition(bp, rel);
    return move_rel(bp);
}

int IAPBoard::move_rel(const BarePosition& rel) const
{
    int ret=0;
    SaveEnvironment();

    for(BarePosition::coord_type::const_iterator
            it = rel.begin(); it != rel.end(); ++it) {
        save_setaxisnum(it->first);
        ret = getAxis(it->first)->move_rel(it->second);
        if (ret < 0)
            break;
    }

    RestoreEnvironment();
    return ret;
}

int IAPBoard::move_to(const Position& abs) const
{
    BarePosition bp;
    conv2bareposition(bp, abs);
    return move_to(bp);
}

int IAPBoard::move_to(const BarePosition& abs) const
{
    int ret = 0;
    SaveEnvironment();

    for(BarePosition::coord_type::const_iterator
            it = abs.begin(); it != abs.end(); ++it) {
        save_setaxisnum(it->first);
        ret = getAxis(it->first)->move_abs(it->second);
        if (ret < 0)
            break;
    }

    RestoreEnvironment();
    return ret;
}


void IAPBoard::get_cur_position(BarePosition& retbarepos) const
{
    //only for PM381
    char buf[128];
    send_command("1QP",buf);
#ifndef SERIAL_DEBUG
    const std::string line(buf);
#else
    // const std::string line("    Channel 5 = 7352\n        Channel 3 = 2000\n        Channel 1 = 3900\n        Channel 4 = 0\n        Channel 2 = 1207\n");
    const std::string line("    Channel 1 = 7352\n        Channel 2 = 2000\n        Channel 3 = 3900\n");
#endif

    const boost::regex re("\\d = [-]?\\d+");

    // for(size_t i=0; i < line.length(); ++i)
    // {
    //     printf("%d:%02x ",i,(unsigned char)line[i]);
    //     if(line[i] == '\n' || line[i] == '\r')
    //         line[i] = ' ';
    // }

    boost::sregex_token_iterator tokens(line.begin(),line.end(),re);
    boost::sregex_token_iterator end;

    for(; tokens != end; ++tokens) {

        std::cout << "gcp tokens: " << *tokens << std::endl;
        std::stringstream ssval(tokens->str().substr(3));
        std::stringstream ssid(tokens->str().substr(0,1));
        BarePosition::type val;
        size_t id;
        ssval >> val;
        ssid >> id;
        if(coordinate_map.find(id) != coordinate_map.end())
            retbarepos.SetCoordinate(id,val);
    }
}

void IAPBoard::get_cur_position(Position& retpos) const
{
    BarePosition bp = createBarePosition();
    get_cur_position(bp);
    conv2postion(retpos, bp);
}

void IAPBoard::get_upper_softlimits(Position& retsl) const
{
  //const std::map<size_t,std::string>& id_string_map = get_coord_map();
  for(const_axesiter it = axes.begin(); it != axes.end(); ++it) {
    retsl.SetCoordinate(it->get_id(),
                        config_.getAxisElement<BarePosition::type>(it->get_id(),"UpperLimit") *
                        it->get_factor());
  }

}

void IAPBoard::get_lower_softlimits(Position& retsl) const
{
  //const std::map<size_t,std::string>& id_string_map = get_coord_map();
  for(const_axesiter it = axes.begin(); it != axes.end(); ++it) {
    retsl.SetCoordinate(it->get_id(),
                        config_.getAxisElement<BarePosition::type>(it->get_id(),"LowerLimit") *
                        it->get_factor());
  }
}

void IAPBoard::reset()
{
    std::cout << "board: reset (not needed at the moment)" << std::endl;
}

//TODO return value
int IAPBoard::SetZero()
{
    std::cout << "board: update current bare position to \'offset\'" << std::endl;

    BarePosition bp = createBarePosition();
    get_cur_position(bp);
    //for(auto& axis : axes) {
    //    axis.SetOffset(bp.GetCoordinate(axis.get_id()));
    //    config_.setAxisElement<BarePosition::type>(axis.get_id(),"UnitConversion.offset",bp.GetCoordinate(axis.get_id()));
    //}
    for(axesiter it = axes.begin(); it != axes.end(); ++it) {
        it->SetOffset(bp.GetCoordinate(it->get_id()));
        getConfig().setAxisElement<BarePosition::type>(it->get_id(),"UnitConversion.offset",bp.GetCoordinate(it->get_id()));
    }
    return 0;
}


int IAPBoard::SetInitialZero()
{
    if(!state())
    {
        std::cerr << "SetInitialZero only in initialize mode allowed!" << std::endl;
        return -1;
    }

    std::cout << "board: initialization: create final parameters.xml file (soft limits set correctly)"
              << std::endl;

    BarePosition bp = createBarePosition();
    get_cur_position(bp);
    //for(auto& axis : axes) {
    for(axesiter it = axes.begin(); it != axes.end(); ++it) {

        config_.setAxisElement<BarePosition::type>(it->get_id(),"Position", 0);
        config_.setAxisElement<BarePosition::type>(it->get_id(),"UnitConversion.offset", 0);

        size_t halfturns = config_.getAxisElement<size_t>(it->get_id(),"init.halfaxisturns");
        double rangefac = config_.getAxisElement<double>(it->get_id(),"init.rangefac");
        BarePosition::type upperSL,lowerSL;
        if(!rangefac)
        {
            // for axes with non-symmetric upper and lower softlimits
            std::cout << "using forward and backward softlimit" << std::endl;
            upperSL = config_.getAxisElement<double>(it->get_id(),"init.constbackwardSoftLimit");
            lowerSL = -1*config_.getAxisElement<double>(it->get_id(),"init.constforwardSoftLimit");
        }
        else{
            //symmetric upper and lower softlimits
            upperSL = static_cast<BarePosition::type>(floor((rangefac/2.0)*halfturns*200));
            lowerSL = -upperSL;
        }
        std::cout << "lowerSL: " << lowerSL << " is [halfturns] " << lowerSL/200.0
                  << "; upperSL: " << upperSL << " is [halfturns]: " << upperSL/200.0 << std::endl;
        config_.setAxisElement<BarePosition::type>(it->get_id(),"UpperLimit", upperSL);
        config_.setAxisElement<BarePosition::type>(it->get_id(),"LowerLimit", lowerSL);
        it->UpdateConfiguration();
    }

    // INITIALISATION DONE -> clear flag
    init_ = false;
    std::cout << "board: done with initailization" << std::endl;
    return 0;
}
