/* -*- c++ -*- */
/*
 * Copyright 2021 gr-pduzmq author.
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

#ifndef INCLUDED_PDUZMQ_NEWZMQ_IMPL_H
#define INCLUDED_PDUZMQ_NEWZMQ_IMPL_H

#include <pduzmq/newzmq.h>
#include <thread>
#include <iostream>
#include <string>
#include <zmq.hpp>
#include <atomic>

namespace gr {
  namespace pduzmq {

    class newzmq_impl : public newzmq
    {
     private:
      // for continuously running function
      gr::thread::thread d_thread;
      std::atomic<bool> d_finished;
      
      int d_timeout; // microseconds, -1 is blocking
      bool d_bind;
      zmq::socket_t d_socket;
      zmq::context_t d_context;

     public:
      newzmq_impl(char* address, int timeout, bool bind);
      ~newzmq_impl();
      
      void run();
      bool start() override;
      bool stop() override;

    };

  } // namespace pduzmq
} // namespace gr

#endif /* INCLUDED_PDUZMQ_NEWZMQ_IMPL_H */

