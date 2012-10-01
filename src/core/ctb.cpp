/*
 * ctb.cpp
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

#include "rs232.hpp"
#include "global.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/algorithm/string.hpp>  //for replace_all

sctl_ctb::sctl_ctb(const boost::property_tree::ptree& config) :
    devicename(config.get<std::string>("devname")),
    baudrate(config.get<int>("baudrate")),
    protocol(config.get<std::string>("protocol")),
    eos(config.get<std::string>("eos")),
    timeout(config.get<unsigned int>("timeout", 50000)), //useconds
    serialPort(STD_TR1::shared_ptr< ctb::SerialPort >(new ctb::SerialPort())),
    device(STD_TR1::shared_ptr< ctb::IOBase >(static_cast<ctb::IOBase*>(0)))
{
    //unescape eos string
    boost::replace_all(eos, "\\\\", "\\");
    boost::replace_all(eos, "\\t",  "\t");
    boost::replace_all(eos, "\\n",  "\n");
    boost::replace_all(eos, "\\r",  "\r");
};


void sctl_ctb::open()
{
    rslog("ctb open");

#ifndef SERIAL_DEBUG // if defined run without hardware
    if( serialPort->Open(devicename.c_str(), baudrate, protocol.c_str(),
                         ctb::SerialPort::NoFlowControl ) >= 0 ) {
        device = serialPort;
    }

    if( ! device ) {
        std::cerr << "Cannot open " <<  devicename  << std::endl;
        exit(1);
    }
#else
    std::cout << "FAKE: " << devicename << " " << baudrate << " "
              << protocol << std::endl;
#endif // SERIAL_DEBUG

    std::cout << "opened " <<  devicename << std::endl;
}


void sctl_ctb::close()
{
    rslog("ctb close");

#ifndef SERIAL_DEBUG // if defined run without hardware
    if( device )
        device->Close();
#endif
}


size_t sctl_ctb::receive(char *buf, const ssize_t n)
{
    size_t size = 0;
    //ssize_t totalsize = 0;

#ifndef SERIAL_DEBUG // if defined run without hardware
    size = device->Read( buf, n);
#else
    static bool rcvflag = true;
    const char teststring[] = "01:TESTSTRING\r\n";

    if(rcvflag) {
        strncpy(buf,teststring,n);
        size = strlen(buf);
    }
    else
        size = 0;

    rcvflag = !rcvflag;
#endif

    // something received?
    if( size > 0 ) {
        //totalsize += size;
        buf[ size ] = 0;
        //std::cout << buf;
        rslog(buf);
    }

    return size;
}


size_t sctl_ctb::send( const char *buf, const ssize_t n)
{
    size_t size = 0;
    // std::cout << "in send:" << buf << std::endl;
#ifndef SERIAL_DEBUG // if defined run without hardware
    size = device->Write( const_cast<char*>(buf), n);
#else
    size = n;
#endif

    // sent string ?
    if( size > 0 ) {
        rslog(buf, "#> ");
    }
    return size;
}
