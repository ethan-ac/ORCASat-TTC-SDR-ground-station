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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "newzmq_impl.h"
// includes for clock
#include <chrono>
#include <iostream>
#include <sys/time.h>
#include <ctime>
#include <vector>
#include <zmq.hpp>

namespace gr {
  namespace pduzmq {

    newzmq::sptr
    newzmq::make(char* address, int timeout, bool bind)
    {
      return gnuradio::get_initial_sptr
        (new newzmq_impl(address, timeout, bind));
    }


    /*
     * The private constructor
     */
    newzmq_impl::newzmq_impl(char* address, int timeout, bool bind)
      : gr::block("newzmq",
                gr::io_signature::make(0, 0, 0),
                gr::io_signature::make(0, 0, 0)),
                d_timeout(timeout)
    {
    /*
    	    int major, minor, patch;
	    zmq::version(&major, &minor, &patch);

	    if (major < 3) {
		d_timeout = timeout * 1000;
	    }
	    
	    zmq::socket_t d_socket(d_context, ZMQ_SUB);
	    d_socket.setsockopt(ZMQ_SUBSCRIBE, "", 0);

	    if (bind) {
		d_socket.bind(address);
	    } else {
		d_socket.connect(address);
	    }
	    // sets up ports to be of type pdu
    	    //message_port_register_out(pmt::mp("out"));
    	    */
    }

    /*
     * Our virtual destructor.
     */
    newzmq_impl::~newzmq_impl()
    {
    }
    
    
    
    
    // starts continuously running function that is used for clock
    bool newzmq_impl::start()
    {
    	d_finished = false;
    	d_thread = gr::thread::thread(std::bind(&newzmq_impl::run, this));
	
    	return block::start();
    }
    
    // stops continuously running function
    bool newzmq_impl::stop()
    {
    	// Shut down the thread
    	d_finished = true;
    	d_thread.interrupt();
    	d_thread.join();

    	return block::stop();
    }
    
    // this is the clock
    // a continuously running function that runs until all current_millis have reached their target_millis
    void newzmq_impl::run()
    {
    	// static_cast<long>(#) sets how often the clock updates in milliseconds
    	// ie #=1 the function runs every millisecond
    	while (!d_finished) {
    		boost::this_thread::sleep(
            	boost::posix_time::milliseconds(static_cast<long>(1)));
        	if (d_finished) {
            		return;
        	}
        	
        	//zmq::message_t zmsg;
        	//socket.recv(zmsg, zmq::recv_flags::none);
        	//std::cout << zmsg << std::endl;
        	//std::cout << "zmsg" << std::endl;
        	
    	// outputs new pdu
    	//message_port_pub(
        //	pmt::mp("out"),
        //	pmt::cons(pmt::car(zmsg), pmt::init_u8vector(zmsg.size(), zmsg)));
    	}
    }
    
    int newzmq_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
    	return 0;
    }
    
  } /* namespace pduzmq */
} /* namespace gr */

