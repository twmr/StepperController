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

namespace helper {
    std::string binary( unsigned long n );

    //return value 0 = Success
    //            <0 = Failure
    template <class T>
    int parse_triple(const std::string str, std::vector<T>& ret)
    {
        bool contains_equalssign = find_if(str.begin(), str.end(),
                                           [](char c) { return( c == '='); }) != str.end();

        if(contains_equalssign) {
            std::cerr << "parse_tripple with equalssign not supported yet" << std::endl;
            return -1; //not supported
        }

        if(ret.size() < 3)
            return -2; //sise of return vector not valid

        std::string word;
        std::stringstream stream(str);

        std::cout << "parse_triple(" << str << ") =";
        for(int i=0; i< 3; ++i) {
            getline(stream, word, ',');
            std::stringstream(word) >> ret[i];
            std::cout << ret[i] << " ";
        }
        std::cout << std::endl;

        return 0;
    }

};

#endif
