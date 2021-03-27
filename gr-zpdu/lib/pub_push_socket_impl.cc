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
#include "pub_push_socket_impl.h"

namespace gr {
  namespace zpdu {

    pub_push_socket::sptr pub_push_socket::make(char* address, int socket, bool link, int timeout)
    {
      return gnuradio::get_initial_sptr (new pub_push_socket_impl(address, socket, link, timeout));
    }


    /*
     * The private constructor
     */
    pub_push_socket_impl::pub_push_socket_impl(char* address, int socket, bool link, int timeout)
      : gr::block("pub_push_socket",
              gr::io_signature::make(0, 0, 0),	// 0's so output can be of type pdu
              gr::io_signature::make(0, 0, 0)),	// 0's so output can be of type pdu
              d_timeout(timeout),
              d_context(1),
              d_socket(d_context, socket)
    {
    	// set timeout based on zmq version
    	int major, minor, patch;
	zmq::version(&major, &minor, &patch);
	if (major < 3) {
		d_timeout = timeout * 1000;
	}
    	
    	// sets up zmq socket to be PUB or PUSH and print info about it to grc terminal
    	int time = 0;
	d_socket.setsockopt(ZMQ_LINGER, &time, sizeof(time));
    	if (socket == ZMQ_PUB) {
	    	std::cout << "PUB socket ";
	}
	else if (socket == ZMQ_PUSH) {
		std::cout << "PUSH socket ";
	}
    	
    	// connect or bind address to zmq socket and print info about it to grc terminal
      	if (link) {
        	d_socket.bind(address);
        	std::cout << "bind to " << address << std::endl << "running..." << std::endl;
    	} else {
        	d_socket.connect(address);
        	std::cout << "connect to " << address << std::endl << "running..." << std::endl;
    	}
    	
    	// sets up input port to be of type pdu
    	message_port_register_in(pmt::mp("in"));
    	set_msg_handler(pmt::mp("in"), [this](pmt::pmt_t msg) { this->msg_handler(msg); });
    }

    /*
     * Our virtual destructor.
     */
    pub_push_socket_impl::~pub_push_socket_impl()
    {
    }
    
    // runs when pdu is received
    // send received pdu to zmq socket
    void pub_push_socket_impl::msg_handler(pmt::pmt_t msg)
    {
    	// convert message from pmt::pmt_t to std::vector<uint8_t>
    	std::vector<uint8_t> umsg = pmt::u8vector_elements(pmt::cdr(msg));
    	
    	// convert message from std::vector<uint8_t> to std::string
    	std::string smsg(umsg.begin(), umsg.end());
    	
    	// convert message from std::string to zmq::message_t
    	zmq::message_t zmsg(smsg.size());
    	
    	// put message received from flowgraph into zmq::message_t
    	memcpy(zmsg.data(), smsg.c_str(), smsg.size());
    	
    	// send message to zmq socket
    	d_socket.send(zmsg, zmq::send_flags::none);
    }

  } /* namespace zpdu */
} /* namespace gr */

