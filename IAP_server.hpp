/*
 * IAP_server.hpp
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

#ifndef __IAPServer__
#define __IAPServer__

#include "global.hpp"
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

namespace IAPServer {
    const int port = 15000;
}

using boost::asio::ip::tcp;

class session {
public:
    session(boost::asio::io_service& io_service)
        : socket_(io_service) { }

    tcp::socket& socket() { return socket_; }

    void start() {
        socket_.async_read_some(boost::asio::buffer(reinterpret_cast<char*>(&data_), msglen),
                                boost::bind(&session::handle_read, this,
                                            boost::asio::placeholders::error,
                                            boost::asio::placeholders::bytes_transferred));
    }

    void process_msg(msg_t& msg);

    // void blocking_send(const std::string& str) {
    //     strcpy(blocking_msg.msg, str.c_str());
    //     blocking_msg.type = MSG_REPLY;
    //     boost::asio::write(socket_, boost::asio::buffer(reinterpret_cast<char*>(&blocking_msg), msglen));
    // }

    /* process_msg triggers functions which call async_send
     * async_send just copies data into the private data_ message object
     */
    void prepare_tcp_message(const std::string& str) {
        strcpy(data_.msg, str.c_str());
        data_.type = MSG_REPLY;
    }

    void prepare_tcp_err_message(const std::string& str) {
        strcpy(data_.msg, str.c_str());
        data_.type = MSG_ERROR;
    }


    void handle_read(const boost::system::error_code& error,
                     size_t bytes_transferred) {
        if (!error)
        {
            std::cout << " new message: [" << static_cast<int>(data_.type)
                      << "] content : " << data_.msg << std::endl;

            process_msg(data_);

            boost::asio::async_write(socket_,
                                     boost::asio::buffer(reinterpret_cast<char*>(&data_), bytes_transferred),
                                     boost::bind(&session::handle_write, this,
                                                 boost::asio::placeholders::error));
        }
        else
            delete this;
    }

    void handle_write(const boost::system::error_code& error) {
        if (!error)
        {
            socket_.async_read_some(boost::asio::buffer(reinterpret_cast<char*>(&data_), msglen),
                                    boost::bind(&session::handle_read, this,
                                                boost::asio::placeholders::error,
                                                boost::asio::placeholders::bytes_transferred));
        }
        else
            delete this;
    }

    // msg_t& get_message() {
    //     return data_;
    // }


private:
    tcp::socket socket_;
    msg_t data_;
    // msg_t blocking_msg;
};

#endif
