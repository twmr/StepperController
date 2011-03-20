/*
 * rs232.cpp
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

#include "rs232.hpp"
#include <ctime>

RS232::RS232(const RS232config & config) :
    rsconfig(config),
    myfilestream("rs232.log", std::ios::out | std::ios::app)
{
    if(myfilestream.is_open() != true) {
	std::cout << "couldn't open rs232 log file" << std::endl;
	abort();
    }
};


int RS232::rslog(const std::string & logstring, const std::string prefix)
{
    struct tm *sec;
    std::time_t mytime;
    char buf[16];

    std::time(&mytime);

    sec = localtime(&mytime);
    strftime(buf,16, "%y%m%02d %H:%M:%S", sec);

    myfilestream << buf << " | "<< prefix << logstring << std::endl;
    myfilestream << std::flush;

    return 0;
}
