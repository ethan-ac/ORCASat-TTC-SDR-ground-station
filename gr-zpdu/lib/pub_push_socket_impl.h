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

#ifndef INCLUDED_ZPDU_PUB_PUSH_SOCKET_IMPL_H
#define INCLUDED_ZPDU_PUB_PUSH_SOCKET_IMPL_H

#include <zpdu/pub_push_socket.h>

// include for zmq
#include <zmq.hpp>

namespace gr {
  namespace zpdu {

    class pub_push_socket_impl : public pub_push_socket
    {
     private:
      // microseconds before .recv() stops listening, -1 is blocking
      int d_timeout;
      
      // setup for zmq socket
      zmq::context_t d_context;
      zmq::socket_t d_socket;

     public:
      pub_push_socket_impl(char* address, int socket, bool link, int timeout);
      ~pub_push_socket_impl();

      // runs when pdu is received
      // send received pdu from input port to zmq socket as zmq message
      void msg_handler(pmt::pmt_t msg);
      
    };

  } // namespace zpdu
} // namespace gr

#endif /* INCLUDED_ZPDU_PUB_PUSH_SOCKET_IMPL_H */

