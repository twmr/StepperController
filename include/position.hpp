/*
 * position.hpp
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

#ifndef __positions__
#define __positions__

#include <boost/lexical_cast.hpp>
#include <iostream>
#include <vector>
#include <exception>
#include <map>

class E_vector_dimension_too_small: public std::exception{
public:
    E_vector_dimension_too_small(){};
    ~E_vector_dimension_too_small() throw() {};
    char const *what() const throw() {return "The provided vector had a too small dimension."; };
};


template <typename T>
class DummyPosition
{
public:
    typedef T type;
    typedef std::map<size_t, std::string> id_desc_map_type;
    typedef std::map<size_t, T> coord_type;

    DummyPosition() :
        coordinates() { };

    DummyPosition(size_t dimension) :
        coordinates()
        {
            for(size_t i = 0; i < dimension; i++) {
                coordinates.insert(std::pair<size_t, T>(i, 0));
            }
        };

    // DummyPosition(const std::map<size_t, std::string> &id_desc_map) :
    //     coordinates()
    //     {
    //         for(std::map<size_t, std::string>::const_iterator itr = id_desc_map.begin();
    //             itr != id_desc_map.end(); ++itr){
    //             coordinates.insert(std::pair<size_t, T>((*itr).first, 0));
    //         }
    //     };

    // DummyPosition(const std::map<std::string, T> &pos) :
    //     coordinates()
    //     {
    //         for(typename std::map<std::string, T>::const_iterator itr = pos.begin();
    //             itr != pos.end(); ++itr){
    //             coordinates.insert(std::pair<size_t, T>(
    //                                    boost::lexical_cast<size_t>((*itr).first), 0));
    //         }
    //     };

    DummyPosition(const std::vector<size_t>& idvec) :
        coordinates()
        {
            for(std::vector<size_t>::const_iterator it = idvec.begin(); it != idvec.end(); ++it) {
                coordinates.insert(std::pair<size_t, T>(*it, 0));
            }
        };

    typename coord_type::iterator begin() { return coordinates.begin(); };
    typename coord_type::const_iterator begin() const { return coordinates.begin(); };
    typename coord_type::iterator end() { return coordinates.end(); };
    typename coord_type::const_iterator end() const { return coordinates.end(); };

    void GetPositionString(char *buf) const
        {
            std::stringstream stream;
            for(typename coord_type::const_iterator itr = begin();
                itr != end(); ++itr){
                stream << "axis" << (*itr).first << ": "
                       << (*itr).second << "\n";
            }

            strcpy(buf, stream.str().c_str());
        };

    void GetPositionString(char *buf, id_desc_map_type& map) const
        {
            std::stringstream stream;
            for(typename coord_type::const_iterator itr = begin();
                itr != end(); ++itr){
                stream << "axis " << map[(*itr).first] << ": "
                       << (*itr).second << "\n";
            }

            strcpy(buf, stream.str().c_str());
        };

    // void PrintPosition(coord_map_type& coord_map) const
    //     {
    //         std::cout << "Position: (";

    //         for(std::map<size_t, std::string>::const_iterator itr = coordinates.begin();
    //             itr != coordinates.end(); ++itr){
    //                       std::cout << coord_map[(*itr).first] << " = "
    //                       << (*itr).second << "\t";
    //         }
    //         std::cout << " )" << std::endl;
    //     };


    void SetCoordinate(const size_t id, const T val)
        {
            if( coordinates.find(id) != coordinates.end())
                coordinates[id] = val;
            else
                coordinates.insert(std::pair<size_t, T>(id,val));
        };

    const T GetCoordinate(const size_t id) const { return coordinates.find(id)->second; };

    // const T GetCoordinate(const std::string desc) const
    //     { return GetCoordinate(GetID(desc)); };




private:
    //FIXME replace this with a hash_map or unordered_map!!!
    size_t GetID(const std::string& desc)
        {
            return 1;
        }

    coord_type coordinates;
};

typedef DummyPosition<short> BarePosition; //units that the stepper board understands
typedef DummyPosition<float> Position; //units that we understand

#endif
