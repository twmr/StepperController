/*
 * IAP_client_cli.cpp
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

#include <fstream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
#include <vector>

#include <boost/asio.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>
#include <boost/tokenizer.hpp>
//#include <boost/thread.hpp>
namespace po = boost::program_options;

#include <readline/readline.h>
#include <readline/history.h>

#include <signal.h>

#include "global.hpp"
#include "IAP_server.hpp"


using boost::asio::ip::tcp;
using namespace std;

class cli_client
{
public:
  cli_client(boost::asio::io_service& io_service,
      tcp::resolver::iterator endpoint_iterator)
    : io_service_(io_service),
      socket_(io_service)
  {

      tcp::endpoint endpoint = *endpoint_iterator;

      //blocking connect
      socket_.connect(endpoint);

//      //async connect
//      socket_.async_connect(endpoint,
//                            boost::bind(&cli_client::handle_connect, this,
//                                        boost::asio::placeholders::error, ++endpoint_iterator));
  }

  ~cli_client()
  {
      do_close();
  }

//  //async version
//  void awrite(const std::string& cmd)
//  {
//      io_service_.post(boost::bind(&cli_client::do_write, this, cmd, true));
//  }

  //blocking wite
  void write(const std::string& cmd)
  {
      cli_client::do_write(cmd,false);
  }

  //blocking read
  std::string read(void)
  {
      size_t reply_length = boost::asio::read(socket_,
                                              boost::asio::buffer(msg_.data(), msg_t::header_length));
      if(reply_length == 0 || !msg_.decode_header())
          return "FAILURE";

      reply_length = boost::asio::read(socket_,
                                       boost::asio::buffer(msg_.body(), msg_.get_body_length()));
      return std::string(msg_.body(), msg_.get_body_length());
  }

  msg_type_t get_type(void) const
  {
      return msg_.get_type();
  }

  void close()
  {
    io_service_.post(boost::bind(&cli_client::do_close, this));
  }

private:

  void handle_connect(const boost::system::error_code& error,
      tcp::resolver::iterator endpoint_iterator)
  {
      if (!error)
      {
          boost::asio::async_read(socket_,
                                  boost::asio::buffer(msg_.data(), msg_t::header_length),
                                  boost::bind(&cli_client::handle_read_header, this,
                                              boost::asio::placeholders::error));
      }
      else if (endpoint_iterator != tcp::resolver::iterator())
      {
          socket_.close();
          tcp::endpoint endpoint = *endpoint_iterator;
          socket_.async_connect(endpoint,
                                boost::bind(&cli_client::handle_connect, this,
                                            boost::asio::placeholders::error, ++endpoint_iterator));
      }
  }

  void handle_read_header(const boost::system::error_code& error)
  {
      if (!error && msg_.decode_header())
      {
          boost::asio::async_read(socket_,
                                  boost::asio::buffer(msg_.body(), msg_.get_body_length()),
                                  boost::bind(&cli_client::handle_read_body, this,
                                              boost::asio::placeholders::error));
      }
      else
      {
          do_close();
      }
  }

  void handle_read_body(const boost::system::error_code& error)
  {
      if (!error)
      {
          std::cout.write(msg_.body(), msg_.get_body_length());
          std::cout << "\n";
      }
      else
      {
          do_close();
      }
  }

  void do_write(const std::string& cmd, bool async)
  {
      using namespace std;
      msg_.set_type(MSG_REQUEST);
      strcpy(msg_.body(),cmd.c_str());
      msg_.set_body_length(cmd.size());
      msg_.encode_header();
      cout << " trying to send message (async): " << cmd
           << "; len: " << msg_.get_body_length()
           << "; " << msg_.body()[0] << msg_.body()[1]
           << endl;

      if(async) {
//          boost::asio::async_write(socket_,
//                                   boost::asio::buffer(msg_.data(),msg_.get_length()),
//                                   boost::bind(&cli_client::handle_write, this,
//                                               boost::asio::placeholders::error));
      }
      else
          boost::asio::write(socket_, boost::asio::buffer(msg_.data(), msg_.get_length()));
  }

//  void handle_write(const boost::system::error_code& error)
//  {
//    if (error)
//    {
//      do_close();
//    }
//  }

  void do_close()
  {
    socket_.close();
  }

private:
  boost::asio::io_service& io_service_;
  tcp::socket socket_;
  msg_t msg_;
};



void catch_int(int sig)
{
    write_history(".stepper.hist");
    exit(0);
}

int main(int argc, char* argv[])
{
    string serverhostname("localhost");
    string serverport("16000"); //string is ok here
    string batch;
    string cmd;
    bool batch_mode = false;
    vector<string> batchcmds;
    bool send_single_cmd = false;

    // Read command line arguments
    try {
        po::options_description desc("Allowed options");
        desc.add_options()
            ("help,h", "produce help message")
            ("serverhostname,s", po::value(&serverhostname),
             "hostname of the server")
            ("serverport,p", po::value(&serverport),
             "portname of the server")
            ("batchfile,b", po::value<string>(),
             "name of the batchfile to execute on the steppermotor hardware")
            ("cmd,c", po::value<string>(),
             "only send a single command to the sever, cmd needs to be put in parenthesis");

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("help")) {
            cout << desc << "\n";
            return 1;
        }
        if (vm.count("batchfile")) {
            //load file
            ifstream ifs(vm["batchfile"].as<string>().c_str());
            if (!ifs) {
                cout << "Could not open the batch file\n";
                return 1;
            }
            // Read the whole file into a string
            stringstream ss;
            ss << ifs.rdbuf();
            // Split the file content
            boost::char_separator<char> sep("\n\r");
            string batchfilecontents( ss.str() );
            boost::tokenizer<boost::char_separator<char> > tok(batchfilecontents, sep);
            copy(tok.begin(), tok.end(), back_inserter(batchcmds));
            batch_mode = true;
        }
        if (vm.count("cmd")) {
            send_single_cmd = true;
            cmd = vm["cmd"].as<string>();
        }
    }
    catch(exception& e) {
        cerr << "error: " << e.what() << "\n";
        return 1;
    }

    try
    {
        boost::asio::io_service io_service;

        tcp::resolver resolver(io_service);
        tcp::resolver::query query(tcp::v4(), serverhostname, serverport);
        tcp::resolver::iterator iterator = resolver.resolve(query);

        tcp::socket s(io_service);

        cli_client c(io_service, iterator);

        //boost::thread t(boost::bind(&boost::asio::io_service::run, &io_service));

        char line[msg_t::max_body_length + 1];

        read_history(".stepper.hist");
        signal(SIGINT, catch_int);

        c.write("state");
        if(boost::starts_with(c.read(),"initialize")) {
            cerr << "client console is disabled during initialization process" << endl;
            return -1;
        }

        if(send_single_cmd) {
            cout << "sending cmd \"" << cmd << "\" to server" << endl;
            c.write(cmd);
            cout << c.read() << endl;
            return 0;
        }

        while(true) {
            if(batch_mode) {
                cout << "#> ";
                if(!batchcmds.size())
                    break;
                cmd = batchcmds.front();
                batchcmds.erase(batchcmds.begin());
                cout << cmd << endl;
            } else
            {
                // interactive mode

                strncpy(line, readline("$>"), msg_t::max_body_length);

                cmd=line;
                if(!cmd.compare("quit") || !cmd.compare("q")) {
                    /* disconnect from tcp/ip server */
                    break;
                }

                if(*line)
                    add_history(line);
            }

            if(!cmd.length()) // empty string
                continue;

            try {
                if (boost::starts_with(cmd, "sleep ")) {
                    int sleep_value = boost::lexical_cast<int>(cmd.substr(6));
                    cout << "sleeping for " << sleep_value << " seconds" << endl;
                    sleep(sleep_value);
                    continue;
                }
            } catch (bad_cast) {
                // bad parameter
                cerr << "bad sleep parameter" << endl;
            }

            c.write(cmd);
            std::string reply = c.read();
            if(c.get_type() == MSG_ERROR)
                cout << "\033[0;31mError: " << reply << "\033[0m" << endl;
            else if(c.get_type() == MSG_SUCCESS)

                cout << "\033[0;32m" << "OK" << "\033[0m" << endl;
            else
                cout << reply << endl;

        }

        c.close();
        //t.join();

    } catch (exception& e) {
        cerr << "Exception: " << e.what() << endl;
    }

    write_history(".stepper.hist");

    return 0;
}
