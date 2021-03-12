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

#ifndef INCLUDED_ZPDU_PUB_PUSH_TEST_IMPL_H
#define INCLUDED_ZPDU_PUB_PUSH_TEST_IMPL_H

#include <zpdu/pub_push_test.h>
#include <iostream>
#include <string>
#include <zmq.hpp>

namespace gr {
  namespace zpdu {

    class pub_push_test_impl : public pub_push_test
    {
     private:
      int d_timeout; // microseconds, -1 is blocking
      zmq::context_t d_context;
      zmq::socket_t d_socket;

     public:
      pub_push_test_impl(char* address, int socket, bool connection, int timeout);
      ~pub_push_test_impl();

      // runs when pdu is received
      // add bytes (preamble/access code) to beginning of pdu
      void msg_handler(pmt::pmt_t msg);

    };

  } // namespace zpdu
} // namespace gr

#endif /* INCLUDED_ZPDU_PUB_PUSH_TEST_IMPL_H */

