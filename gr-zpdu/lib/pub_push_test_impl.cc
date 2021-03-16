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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "pub_push_test_impl.h"
#include <iostream>
#include <string>
#include <typeinfo>
// include for zmq
#include <zmq.hpp>

namespace gr {
  namespace zpdu {

    pub_push_test::sptr pub_push_test::make(char* address, int socket, bool connection, int timeout)
    {
      return gnuradio::get_initial_sptr (new pub_push_test_impl(address, socket, connection, timeout));
    }


    /*
     * The private constructor
     */
    pub_push_test_impl::pub_push_test_impl(char* address, int socket, bool connection, int timeout)
      : gr::block("pub_push_test",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0)),
              d_timeout(timeout),
              d_context(1),
              d_socket(d_context, socket)
    {
    	// set timeout properly based on zmq version
    	int major, minor, patch;
	zmq::version(&major, &minor, &patch);
	if (major < 3) {
		d_timeout = timeout * 1000;
	}
    	
    	// sets up input port to be of type pdu
    	message_port_register_in(pmt::mp("in"));
    	set_msg_handler(pmt::mp("in"), [this](pmt::pmt_t msg) { this->msg_handler(msg); });
    	
    	// sets up socket to be PUB or PUSH
    	int time = 0;
	d_socket.setsockopt(ZMQ_LINGER, &time, sizeof(time));
    	if (socket == ZMQ_PUB) {
	    	std::cout << "PUB socket ";
	}
	else if (socket == ZMQ_PUSH) {
		std::cout << "PUSH socket ";
	}
    	
    	// connect or bind address to zmq socket and print info about it to grc terminal
      	if (connection) {
        	d_socket.bind(address);
        	std::cout << "bind to " << address << std::endl << "running..." << std::endl;
    	} else {
        	d_socket.connect(address);
        	std::cout << "connect to " << address << std::endl << "running..." << std::endl;
    	}
    }

    /*
     * Our virtual destructor.
     */
    pub_push_test_impl::~pub_push_test_impl()
    {
    }
    
    // runs when pdu is received
    // send received pdu to zmq socket
    void pub_push_test_impl::msg_handler(pmt::pmt_t msg)
    {
    	// convert from pmt::pmt_t to std::vector<uint8_t>
    	std::vector<uint8_t> pmsg = pmt::u8vector_elements(pmt::cdr(msg));
    	// convert from std::vector<uint8_t> to std::string
    	std::string smsg(pmsg.begin(), pmsg.end());
    	// convert from std::string to zmq::message_t
    	zmq::message_t zmsg(smsg.size());
    	// put message received from flowgraph into zmq::message_t
    	memcpy(zmsg.data(), smsg.c_str(), smsg.size());
    	// send message over zmq socket
    	d_socket.send(zmsg, zmq::send_flags::none);
    }

  } /* namespace zpdu */
} /* namespace gr */

