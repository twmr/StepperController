/*
 * helper.hpp
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

#ifndef __HELPER__
#define __HELPER__

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <map>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp> // for trim function

namespace helper {
    using namespace std;

    string binary( unsigned long n );
    bool string_contains(const string& str, const char c);

    //return value 0 = Success
    //            <0 = Failure
    template <class T>
    int parse_multiplett(const string str, map<string,T>& ret, const char delim='=')
    {
        using namespace boost;
        cout << "parse_multiplett(" << str << ") " << endl;


        if(!string_contains(str,delim)) {
            cerr << "parse_multiplet without delimiter not supported yet" << endl;
            return -1; //not supported
        }

        tokenizer<escaped_list_separator<char> > tok(str);
        for(tokenizer<escaped_list_separator<char> >::iterator beg=tok.begin(); beg!=tok.end();++beg){
            // cout << *beg << "\n";
            string var = beg->substr(0, beg->find(delim));
            string value = beg->substr(beg->find(delim)+1);

            // remove whitespaces
            trim(var);
            trim(value);

            T tmp;
            stringstream(value) >> tmp;
            ret.insert(pair<string,T>(var,tmp));
            cout << "var : " << var << " = " << tmp  << "\n";
        }

        return 0;
    }
};

#endif
