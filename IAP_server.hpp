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
    void async_send(const std::string& str) {
        strcpy(data_.msg, str.c_str());
        data_.type = MSG_REPLY;
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
