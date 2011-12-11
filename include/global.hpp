/*
 * global.hpp
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

#ifndef __SCTL_global__
#define __SCTL_global__

#ifdef USE_BOOST
 #include <boost/shared_ptr.hpp>
 #include <boost/weak_ptr.hpp>
 #define STD_TR1 boost
#else
 #include <tr1/memory>
 #define STD_TR1 std::tr1
#endif

typedef enum {
    UNKNOWN_CMD,
    INVAL_PREFIX,
    INVAL_ARG,
    TIMEOUT,
    CON_ERROR
} server_error_t;

#endif
