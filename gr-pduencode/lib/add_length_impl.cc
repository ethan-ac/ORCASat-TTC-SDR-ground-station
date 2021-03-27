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
#include "add_length_impl.h"

namespace gr {
  namespace pduencode {

    add_length::sptr add_length::make(bool crc16_present)
    {
      return gnuradio::get_initial_sptr (new add_length_impl(crc16_present));
    }


    /*
     * The private constructor
     */
    add_length_impl::add_length_impl(bool crc16_present)
      : gr::block("add_length",
              gr::io_signature::make(0, 0, 0),	// 0's so input can be of type pdu 
              gr::io_signature::make(0, 0, 0)),	// 0's so output can be of type pdu
              d_crc16_present(crc16_present)
    {
    	// sets up ports to be of type pdu
    	message_port_register_out(pmt::mp("out"));
    	message_port_register_in(pmt::mp("in"));
    	set_msg_handler(pmt::mp("in"), [this](pmt::pmt_t msg) { this->msg_handler(msg); });
    }

    /*
     * Our virtual destructor.
     */
    add_length_impl::~add_length_impl()
    {
    }

    // runs when pdu is received
    // calculates then inserts length field to head of pdu and outputs
    void add_length_impl::msg_handler(pmt::pmt_t pmt_msg)
    {
    	// convert received pdu from pdu to std::vector<uint8_t>
    	std::vector<uint8_t> msg = pmt::u8vector_elements(pmt::cdr(pmt_msg));
    	
    	// calculates then inserts length field into head of pdu
    	// if crc16 isnt added yet but needs to be included in the length field add 2 to length
    	if (d_crc16_present) {
	    	msg.insert(msg.begin(), msg.size()+2);
    	} else {
	    	msg.insert(msg.begin(), msg.size());
    	}
    
    	// outputs new pdu to output port
    	message_port_pub(
        	pmt::mp("out"),
        	pmt::cons(pmt::car(pmt_msg), pmt::init_u8vector(msg.size(), msg)));
    }

  } /* namespace pduencode */
} /* namespace gr */
