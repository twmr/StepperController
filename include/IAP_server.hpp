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

#include "message.hpp"
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

namespace IAPServer {
    using boost::asio::ip::tcp;

    class session {
    public:
        session(boost::asio::io_service& io_service)
            : socket_(io_service) { }

        tcp::socket& socket() { return socket_; }

        void start() {
            socket_.async_read_some(boost::asio::buffer(data_.data(), msg_t::header_length),
                                    boost::bind(&session::handle_read_header, this,
                                                boost::asio::placeholders::error));
        }

        void process_msg(msg_t& msg);

        /* process_msg triggers functions which call async_send
         * async_send just copies data into the private data_ message object
         */
        void prepare_tcp_message(const std::string& str) {
            strcpy(data_.body(), str.c_str());
            data_.set_type(MSG_REPLY);
        }

        void prepare_tcp_err_message(const std::string& str) {
            strcpy(data_.body(), str.c_str());
            data_.set_type(MSG_ERROR);
        }

        void prepare_tcp_success_message() {
            data_.body()[0]='\0';
            data_.set_type(MSG_SUCCESS);
        }

        void handle_read_header(const boost::system::error_code& error) {
            if (!error && data_.decode_header())
            {
                std::cout << " header_received [" << data_.get_type()
                          << "] with length : " << data_.get_body_length() << std::endl;

                boost::asio::async_read(socket_,
                                             boost::asio::buffer(data_.body(), data_.get_body_length()),
                                             boost::bind(&session::handle_read_body, this,
                                                         boost::asio::placeholders::error));
            }
            else
                delete this;
        }

        void handle_read_body(const boost::system::error_code& error) {
            if (!error)
            {
                //this function does all the work and updates the body and the type of the return message
                process_msg(data_);

                //update len field of header
                //assumes that process_msg 0-terminates the body
                data_.set_body_length(strlen(data_.body()));
                data_.encode_header();

                boost::asio::async_write(socket_,
                                         boost::asio::buffer(data_.data(), data_.get_length()),
                                         boost::bind(&session::handle_write, this,
                                                     boost::asio::placeholders::error));
            }
            else
                delete this;
        }

        void handle_write(const boost::system::error_code& error) {
            if (!error)
            {
                socket_.async_read_some(boost::asio::buffer(data_.data(),msg_t::header_length),
                                        boost::bind(&session::handle_read_header, this,
                                                    boost::asio::placeholders::error));
            }
            else
                delete this;
        }

    private:
        tcp::socket socket_;
        msg_t data_;
    };
}


#endif
