/* -*- c++ -*- */
/*
 * Copyright 2021 gr-pduencode author.
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
#include "length_impl.h"
#include <algorithm>
#include <vector>
#include <string>

namespace gr {
  namespace pduencode {

    length::sptr length::make()
    {
      return gnuradio::get_initial_sptr (new length_impl());
    }


    /*
     * The private constructor
     */
    length_impl::length_impl()
      : gr::block("length",
              gr::io_signature::make(0, 0, 0),	// 0's so input can be of type pdu 
              gr::io_signature::make(0, 0, 0))	// 0's so output can be of type pdu
    {
    	// sets up ports to be of type pdu
    	message_port_register_out(pmt::mp("out"));
    	message_port_register_in(pmt::mp("in"));
    	set_msg_handler(pmt::mp("in"), [this](pmt::pmt_t msg) { this->msg_handler(msg); });
    }

    /*
     * Our virtual destructor.
     */
    length_impl::~length_impl()
    {
    }
    
    // runs when pdu is received
    // calculates then adds length field to beginning of pdu
    void length_impl::msg_handler(pmt::pmt_t pmt_msg)
    {
    	// convert received pdu to type that can be manipulated
    	std::vector<uint8_t> msg = pmt::u8vector_elements(pmt::cdr(pmt_msg));
    	std::vector<uint8_t> cut_msg = std::vector<uint8_t>(msg.begin(), msg.end());
    	
    	// inserts length field into beginning of pdu
    	cut_msg.insert(cut_msg.begin(), cut_msg.size());
    
    	// outputs new pdu
    	message_port_pub(
        	pmt::mp("out"),
        	pmt::cons(pmt::car(pmt_msg), pmt::init_u8vector(cut_msg.size(), cut_msg)));
    }

  } /* namespace pduencode */
} /* namespace gr */

