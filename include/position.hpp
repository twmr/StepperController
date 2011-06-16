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

//#include <boost/tuple/tuple.hpp>
#include <iostream>
#include <vector>
#include <exception>

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
    DummyPosition() :
        coordinates(3)
        { };

    DummyPosition(T x, T y, T t) :
        coordinates(3)
        {
            coordinates[0] = x;
            coordinates[1] = y;
            coordinates[2] = t;
        };

    //DummyPosition(const boost::tuple<T,T,T>& tp) :
    //    x_(tp.get(0)), y_(tp.get(1)), theta_(tp.get(2))
    //    { };

    DummyPosition(const std::vector<T>& tvec) :
        coordinates(tvec)
        {
            if (coordinates.size() < 3) {
                throw E_vector_dimension_too_small();
            }
        };

    void PrintPosition() const
        {
            std::cout << "Position: ( x: " << get_x() << " y: " << get_y() << " theta: "
                      << get_theta() << " )" << std::endl;
        };

    void SetPosition(const T x, const T y, const T t)
        {
            coordinates[0] = x;
            coordinates[1] = y;
            coordinates[2] = t;
        }

    const T get_x() const { return coordinates[0]; };
    const T get_y() const { return coordinates[1]; };
    const T get_theta() const { return coordinates[2]; };

    typedef T type;

private:
    //TODO replace this with a hash_map or unordered_map!!!
    std::vector<T> coordinates;
};

typedef DummyPosition<short> BarePosition; //units that the stepper board understands
typedef DummyPosition<float> Position; //units that we understand

class ConversionConstants
{
public:
    ConversionConstants() :
        xconv_(1.0), yconv_(1.0), thetaconv_(1.0)
        { };

    ConversionConstants(double xc, double yc, double thetac) :
        xconv_(xc), yconv_(yc), thetaconv_(thetac)
        { };

    void get_bare_position(BarePosition& ret, const Position &pos) const;
    void get_position(Position& ret, const BarePosition &bpos) const;
    void set_constants(const std::vector<double>& cvec);
    double xconv_;
    double yconv_;
    double thetaconv_;
};

#endif
