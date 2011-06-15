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

sctl_ctb::sctl_ctb(const RS232config & config) :
    RS232(config),
    serialPort(STD_TR1::shared_ptr< ctb::SerialPort >(new ctb::SerialPort())),
    device(STD_TR1::shared_ptr< ctb::IOBase >(static_cast<ctb::IOBase*>(0)))
{
}


void sctl_ctb::open()
{
    rslog("ctb open");

    if( serialPort->Open( rsconfig.get_string_param("devname").c_str(),
        rsconfig.get_int_param("baudrate"),
        rsconfig.get_string_param("protocol").c_str(),
        ctb::SerialPort::NoFlowControl ) >= 0 ) {
        device = serialPort;
    }

    if( ! device ) {
        std::cerr << "Cannot open " <<  rsconfig.get_string_param("devname")  << std::endl;
        exit(1);
    }
    std::cout << "opened " <<  rsconfig.get_string_param("devname")  << std::endl;
}


void sctl_ctb::close()
{
    rslog("ctb close");

    if( device )
        device->Close();
}


size_t sctl_ctb::receive(char *buf, const ssize_t n)
{
    size_t size = 0;
    //ssize_t totalsize = 0;

    size = device->Read( buf, n);

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

    size = device->Write( const_cast<char*>(buf), n);

    // sent string ?
    if( size > 0 ) {
        rslog("#> ", buf);
    }
    std::cout << "write returned" << size << std::endl;
    return size;
}
