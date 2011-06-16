/*
 * position.cpp
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

#include "position.hpp"

void  ConversionConstants::get_bare_position(BarePosition& ret, const Position &pos) const
{
    ret.SetPosition(
        static_cast<BarePosition::type>(pos.get_x()/xconv_),
        static_cast<BarePosition::type>(pos.get_y()/yconv_),
        static_cast<BarePosition::type>(pos.get_theta()/thetaconv_));
}

void ConversionConstants::get_position(Position& ret, const BarePosition &bpos) const
{
    ret.SetPosition(bpos.get_x()*xconv_,
                    bpos.get_y()*yconv_,
                    bpos.get_theta()*thetaconv_);
}

void ConversionConstants::set_constants(const std::vector<double>& cvec)
{
    if(cvec.size() < 3){
        std::cerr << "WARNING: vector length in set_constants too small" << std::endl;
        return;
    }

    xconv_ = cvec[0];
    yconv_ = cvec[1];
    thetaconv_ = cvec[2];
}
