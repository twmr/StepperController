/*
 * exceptions.hpp
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

#ifndef EXC_TRFILE_H
#define EXC_TRFILE_H

#include <string>
#include <exception>

class E_invalid_param_value: public std::exception{
public:    
    const char* object_name;
    const char* param_name;

    explicit E_invalid_param_value(const char* pn, const char* on): 
	object_name(on), param_name(pn){};
    const char* what() const throw(){
	return "Invalid Parameter value.\n";
    };
};

class E_invalid_dynamic_cast: public std::exception{
public:    
    const char* Obj_name;

    explicit E_invalid_dynamic_cast(const char* on): 
	Obj_name(on){};
    const char* what() const throw(){
	return "Essential Parameter missing.\n";
    };
};

class E_missing_parameter: public std::exception{
public:
    const char* object_name;
    const char* param_name;

    explicit E_missing_parameter(const char* pn, const char* on): 
	object_name(on), param_name(pn){};
    const char* what() const throw(){
	return "Essential Parameter missing.\n";
    };
};

class E_missing_XML_attribute: public std::exception{
public:
    const std::string key;

    explicit E_missing_XML_attribute(const std::string& k): 
	key(k) {};
    ~E_missing_XML_attribute() throw() {};
    const char* what() const throw(){
	return "XML attribute missing.\n";
    };
};

class E_double_XML_name: public std::exception{
public:
    const std::string name;

    explicit E_double_XML_name(const std::string& k): 
	name(k) {};
    ~E_double_XML_name() throw() {};
    const char* what() const throw(){
	return "Same XML object name used twice.\n";
    };
};

class E_XML_mixup: public std::exception{
public:
    const std::string section;
    explicit E_XML_mixup(const std::string& s): 
	section(s) {};
    ~E_XML_mixup() throw() {};
    const char* what() const throw(){
	return "XML Section \"object\" unexpected.\n";
    };
};

class E_XML_unknown: public std::exception{
public:
    const std::string section;
    explicit E_XML_unknown(const std::string& s): 
	section(s) {};
    ~E_XML_unknown() throw() {};
    const char* what() const throw(){
	return "XML object \"object\" unknown.\n";
    };
};

class E_unknown_typename: public std::exception{
public:
    std::string type_name;
    explicit E_unknown_typename(const std::string& tn):type_name(tn){};
    ~E_unknown_typename() throw() {};
    const char* what() const throw(){
	return "Typename identifier not known!\n";
    };
};

class E_circular_reference: public std::exception{
public:
    std::string obj_class;
    explicit E_circular_reference(const std::string& tn):obj_class(tn){};
    ~E_circular_reference() throw() {};
    const char* what() const throw(){
	return "Circular reference in object dependencies!\n";
    };
};

class E_Not_ready: public std::exception{
public:
    const char* object;

    explicit E_Not_ready(const char* c):object(c){};
    const char* what()  const throw (){return "Object not initialized!";};
};

class E_unknown_command: public std::exception{
    std::string k;
 public:
    E_unknown_command(const std::string& s):k(s){};
    ~E_unknown_command() throw() {};
    char const *what() const throw() {return "The provided command is unknown.";};
    const std::string& Command_name() const throw() {return k;};
};

class E_syntax_error: public std::exception{
 public:
    E_syntax_error(){};
    ~E_syntax_error() throw() {};
    char const *what() const throw() {return "Syntax error!";};
};

class E_incompatible_average: public std::exception{
    std::string k;
 public:
    E_incompatible_average(const std::string& s):k(s){};
    ~E_incompatible_average() throw() {};
    char const *what() const throw() {return "The average over these two datasets is impossible.";};
    const std::string& H_name() const throw() {return k;};
};

class E_key_not_found: public std::exception{
    std::string k;
 public:
    E_key_not_found(const std::string& s):k(s){};
    ~E_key_not_found() throw() {};
    char const *what() const throw() {return "The provided key was not found.";};
    const std::string& key_name() const throw() {return k;};
};

class E_invalid_filename: public std::exception{
    std::string k;
 public:
    E_invalid_filename(const std::string& s):k(s){};
    ~E_invalid_filename() throw() {};
    char const *what() const throw() {return "The provided filename is invalid.";};
    const std::string& filename() const throw() {return k;};
};

class E_uninintialized_Rect: public std::exception{
    int k;
 public:
    E_uninintialized_Rect(const int i):k(i){};
    ~E_uninintialized_Rect() throw() {};
    char const *what() const throw() {return "The Rectangle is uninizialised.";};
    const int Rect_Nr() const throw() {return k;};
};

class E_invalid_TRfile: public std::exception{
 public:
    E_invalid_TRfile(){};
    ~E_invalid_TRfile() throw() {};
    char const *what() const throw() {return "The provided TRfile could not be loaded.";};
};

class E_bad_index: public std::exception{
 public:
    int index;
    E_bad_index():index(0){};
    E_bad_index(const int i):index(i){};
    ~E_bad_index() throw() {};
    char const *what() const throw() {return "The provided index is out of range.";};
};

class E_bad_range_string: public std::exception{
    std::string k;
 public:
    E_bad_range_string(const std::string& s):k(s){};
    ~E_bad_range_string() throw() {};
    char const *what() const throw() {return "The provided range string "
					  "could not be parsed.";};
    const std::string& parse_string() const throw() {return k;};
};

class E_index_out_of_range: public std::exception{
    size_t val;
    size_t max;
public:
    E_index_out_of_range(size_t v, size_t m):val(v), max(m){};
    ~E_index_out_of_range() throw() {};
    char const *what() const throw() {return "The provided index was out of range.";};
    size_t get_val(){return val;};
    size_t get_max(){return max;};
};
	

#endif
