/*
 * rs232.hpp
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

#ifndef __RS232__
#define __RS232__

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <boost/property_tree/ptree_fwd.hpp>

#include "ctb-0.15/ctb.h"
#include "global.hpp"
#include "exceptions.hpp"

class RS232 {
public:
    RS232();
    virtual ~RS232() { if(myfilestream.is_open()) myfilestream.close(); };

    // pure virtual functions
    virtual void open() = 0;
    virtual void close() = 0;
    virtual size_t send(const char *buf, ssize_t n) = 0;
    virtual size_t receive(char *buf, const ssize_t n) = 0;

    int rslog(const std::string & logstring, const std::string prefix = "");
private:
    std::ofstream myfilestream;
};

//use libctb for rs232
class sctl_ctb: public RS232 {
public:
    sctl_ctb(const boost::property_tree::ptree& pt);
    ~sctl_ctb() {};
    void open();
    void close();
    //int send() {};
    //int receive() {};
    size_t send(const char *buf, const ssize_t n);
    size_t receive(char *buf, const ssize_t n);

private:
    const std::string devicename;
    const int baudrate;
    const std::string protocol;

    STD_TR1::shared_ptr< ctb::SerialPort > serialPort;
    STD_TR1::shared_ptr< ctb::IOBase > device;
};

#endif
