/* -*- c++ -*- */
/*
 * Copyright 2021 ethan.
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_ZPDU_SUB_PULL_SOCKET_IMPL_H
#define INCLUDED_ZPDU_SUB_PULL_SOCKET_IMPL_H

#include <zpdu/sub_pull_socket.h>

// include for zmq
#include <zmq.hpp>

namespace gr {
  namespace zpdu {

    class sub_pull_socket_impl : public sub_pull_socket
    {
     private:
      // for continuously running function
      gr::thread::thread d_thread;
      std::atomic<bool> d_finished;
      
      // microseconds before .recv() stops listening, -1 is blocking
      int d_timeout;
      
      // setup for zmq socket
      zmq::context_t d_context;
      zmq::socket_t d_socket;

     public:
      sub_pull_socket_impl(char* address, int socket, bool link, int timeout);
      ~sub_pull_socket_impl();

      // runs when pdu is received
      // send received zmq message from zmq socket to output port as pdu
      void msg_handler(std::string msg);
      
      // continuously running function
      // receives zmq message from zmq socket
      void run();
      bool start() override;
      bool stop() override;

    };

  } // namespace zpdu
} // namespace gr

#endif /* INCLUDED_ZPDU_SUB_PULL_SOCKET_IMPL_H */

