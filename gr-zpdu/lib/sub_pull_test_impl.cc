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
#include "sub_pull_test_impl.h"
// includes for continuously running function
#include <atomic>
#include <chrono>
#include <iostream>
#include <sys/time.h>
#include <ctime>
#include <vector>
#include <string>
// include for zmq
#include <zmq.hpp>

namespace gr {
  namespace zpdu {

    sub_pull_test::sptr sub_pull_test::make(char* address, int socket, bool connection, int timeout)
    {
      return gnuradio::get_initial_sptr (new sub_pull_test_impl(address, socket, connection, timeout));
    }


    /*
     * The private constructor
     */
    sub_pull_test_impl::sub_pull_test_impl(char* address, int socket, bool connection, int timeout)
      : gr::block("sub_pull_test",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0)),
              d_timeout(timeout),
              d_finished(false),
              d_context(1),
              d_socket(d_context, socket)
    {
    	// set timeout properly based on zmq version
    	int major, minor, patch;
	zmq::version(&major, &minor, &patch);
	if (major < 3) {
		d_timeout = timeout * 1000;
	}
	
    	// sets up output port to be of type pdu
    	message_port_register_out(pmt::mp("out"));
    	
    	// sets up socket to be SUB or PULL
    	if (socket == ZMQ_SUB) {
    		d_socket.setsockopt(ZMQ_SUBSCRIBE, "", 0);
    		std::cout << "SUB socket ";
    	}
    	else if (socket == ZMQ_PULL) {
    		int time = 0;
    		d_socket.setsockopt(ZMQ_LINGER, &time, sizeof(time));
    		std::cout << "PULL socket ";
    	}
    	
    	// connect/bind address to zmq socket and print info about it to grc terminal
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
    sub_pull_test_impl::~sub_pull_test_impl()
    {
    }
    
    // starts continuously running function
    bool sub_pull_test_impl::start()
    {
    	d_finished = false;
    	d_thread = gr::thread::thread(std::bind(&sub_pull_test_impl::run, this));
	
    	return block::start();
    }
    
    // stops continuously running function
    bool sub_pull_test_impl::stop()
    {
    	// Shut down the thread
    	d_finished = true;
    	d_thread.interrupt();
    	d_thread.join();

    	return block::stop();
    }
    
    // continuously running function
    // receives data from zmq socket
    void sub_pull_test_impl::run()
    {
    	// static_cast<long>(#) sets how often the clock updates in milliseconds
    	// ie #=1 the function runs every millisecond
    	while (!d_finished) {
    		boost::this_thread::sleep(
            	boost::posix_time::milliseconds(static_cast<long>(1)));
        	if (d_finished) {
            		return;
        	}
        	// prevents function from getting stuck on .recv() function so flowgraph can be shut down smoothly
        	// otherwise after clicking "stop" button must send another message to shutdown
        	zmq::pollitem_t items[] = { { static_cast<void*>(d_socket), 0, ZMQ_POLLIN, 0 } };
        	zmq::poll(&items[0], 1, d_timeout);
        	if (items[0].revents & ZMQ_POLLIN) {
			// receive a message via zmq
			zmq::message_t zmsg;
			d_socket.recv(zmsg, zmq::recv_flags::none);
			// convert received msg, zmq::message_t to std::string
			std::string msg(static_cast<char*>(zmsg.data()), zmsg.size());
			// send to pdu output port
	   		msg_handler(msg);
	   	} else {
		    	boost::this_thread::sleep(
		    	boost::posix_time::microseconds(static_cast<long>(100)));
            	}
        }
    }
    
    // runs when pdu is received
    // send received zmq data to output port as pdu
    void sub_pull_test_impl::msg_handler(std::string msg)
    {
    	// converts string to uint8_t vector
    	std::vector<uint8_t> vec(msg.begin(), msg.end());
    	// makes and outputs new pdu
    	message_port_pub(
        	pmt::mp("out"),
        	pmt::cons(pmt::PMT_NIL, pmt::init_u8vector(vec.size(), vec)));
    }

  } /* namespace zpdu */
} /* namespace gr */

