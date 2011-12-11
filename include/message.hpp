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

#ifndef __SCTL_MESSAGE__
#define __SCTL_MESSAGE__

#include "global.hpp"

typedef enum {
    MSG_ERROR,
    MSG_NONE,
    MSG_REQUEST, //command message
    MSG_REPLY,
    MSG_SUCCESS,
    MSG_INVALID
} msg_type_t;

class msg_t
{
public:
  enum { type_length = 4 };
  enum { len_length = 4 };
  enum { header_length = type_length + len_length };
  enum { max_body_length = 1024 };

  msg_t()
    : body_length_(0),
      tp(MSG_INVALID)
  {
  }

  const char* data() const
  {
    return data_;
  }

  char* data()
  {
    return data_;
  }



  const char* body() const
  {
    return data_ + header_length;
  }

  char* body()
  {
    return data_ + header_length;
  }

  const char* hdr_len() const
  {
    return data_ + type_length;
  }

  char* hdr_len()
  {
    return data_ + type_length;
  }

  //total message length
  size_t get_length() const
  {
    return header_length + body_length_;
  }

  //length of the body of the message
  size_t get_body_length() const
  {
    return body_length_;
  }

  void set_body_length(size_t length)
  {
    body_length_ = length;
    if (body_length_ > max_body_length)
      body_length_ = max_body_length;
  }

  bool decode_header()
  {
    using namespace std; // For strncat and atoi.
    char header[len_length + 1] = ""; //length of body
    strncat(header, hdr_len(), len_length);
    body_length_ = atoi(header);
    if (body_length_ > max_body_length)
    {
      body_length_ = 0;
      return false;
    }

    //get type
    memcpy(header, data(), type_length);
    header[type_length] = '\0';
    unsigned int type = atoi(header);
    if (type >= static_cast<unsigned int>(MSG_INVALID))
    {
        tp=MSG_INVALID;
        return false;
    }
    else
        tp=static_cast<msg_type_t>(type);

    cout << "decode: " << body_length_ << " " << tp << endl;
    return true;
  }

  void encode_header()
  {
    using namespace std; // For sprintf and memcpy.

    //encode type
    char header_type[type_length + 1] = "";
    sprintf(header_type, "%04d", tp);
    memcpy(data_, header_type, type_length);

    //encode length
    char header_len[len_length + 1] = "";
    sprintf(header_len, "%04u", static_cast<unsigned int>(body_length_));
    memcpy(hdr_len(), header_len, len_length);

    cout << "encode type: " << header_type << "; length:" << header_len << endl;
  }

  msg_type_t get_type(void) const { return tp; }
  void set_type(msg_type_t t) { tp = t; }

 private:
    char data_[header_length + max_body_length];
    size_t body_length_;
    msg_type_t tp;
};

#endif
