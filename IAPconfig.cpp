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
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>

IAPconfig::IAPconfig(const std::string& configfile)
{
    std::ifstream f(configfile.c_str());
    char cstring[256];
    char * pch;

    std::cout << "opening IAP board axis config file: " << configfile << std::endl;
    if( f.is_open() != true) {
        std::cerr << "couldn't open IAP config file" << std::endl;
        abort();
    }

    while (!f.eof())
    {
        f.getline(cstring, sizeof(cstring));
        pch=strchr(cstring,'=');
        if (cstring[0] != '#' && pch != NULL)
        {
            *pch = '\0';

            if(!strcmp(cstring,"eos")){
                int n;
                //char tmp[16];
                //strncpy(tmp,pch+1,strlen(pch+1));
                //n = sprintf(pch+1, tmp);
                n = sprintf(pch+1, "\r\n");
                if(n > 0)
                    *(pch+1+n)='\0';
            }

            s_params.insert(std::pair<std::string,std::string>(cstring, pch+1));
        }
    }
    f.close();

    //std::map<std::string, std::string>::const_iterator p = s_params.begin();
    //for (; p != s_params.end(); ++p)
    //std::cout << " result: "<< p->first << " = " << p->second << std::endl;

}

double IAPconfig::get_double_param(const std::string& s) const throw (E_missing_parameter)
{
    double d;
    if (!get_param(s,d)) 
        throw (E_missing_parameter(s.c_str(),name()));
    return d;
}

int IAPconfig::get_int_param(const std::string& s) const throw (E_missing_parameter)
{
    int d;
    if (!get_param(s,d)) 
        throw (E_missing_parameter(s.c_str(),name()));
    return d;
}

std::string IAPconfig::get_string_param(const std::string& s) const throw (E_missing_parameter)
{
    std::string d;
    if (!get_param(s,d)) 
        throw (E_missing_parameter(s.c_str(),name()));
    return d;
}


bool IAPconfig ::get_param(const std::string& s, double& v) const{
    std::map<std::string, std::string>::const_iterator p = s_params.find(s);
    if (p == s_params.end())
        return false;
    double tmp;
    sscanf(p->second.c_str(), "%lf", &tmp);
    v = tmp;
    return true;
}

bool IAPconfig::get_param(const std::string& s, int& v) const{
    std::map<std::string, std::string>::const_iterator p = s_params.find(s);
    if (p == s_params.end())
        return false;
    v = atoi((p->second).c_str());
    return true;
}

bool IAPconfig::get_param(const std::string& s, std::string& v) const{
    std::map<std::string, std::string>::const_iterator p = s_params.find(s);
    if (p == s_params.end())
        return false;
    v = p->second;
    return true;
}
