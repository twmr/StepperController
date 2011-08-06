/*
 * IAPconfig.cpp
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

#include "IAPBoard.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

using namespace std;


IAPconfig::IAPconfig(const std::string& configfile) :
    params_(),
    filename(configfile)
{
    params_.clear();
    read_xml(configfile, params_);
}


void IAPconfig::writeconfig() const
{
    //boost::property_tree::xml_writer_settings<char> settings('\t', 1);
    boost::property_tree::xml_writer_settings<char> settings(' ', 2);
    write_xml(filename, params_, std::locale(), settings);
}
