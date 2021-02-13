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
#include "bytes_impl.h"
#include <algorithm>
#include <vector>
#include <string>

namespace gr {
  namespace pduencode {

    bytes::sptr bytes::make(std::string add)
    {
    	return gnuradio::get_initial_sptr (new bytes_impl(add));
    }


    /*
     * The private constructor
     */
    bytes_impl::bytes_impl(std::string add)
      : gr::block("bytes",
              gr::io_signature::make(0, 0, 0),	// 0's so input can be of type pdu 
              gr::io_signature::make(0, 0, 0)),	// 0's so output can be of type pdu
              d_add(add)	// string to be added to pdu
    {
    	// sets up ports to be of type pdu
    	message_port_register_out(pmt::mp("out"));
    	message_port_register_in(pmt::mp("in"));
    	set_msg_handler(pmt::mp("in"), [this](pmt::pmt_t msg) { this->msg_handler(msg); });
    }

    /*
     * Our virtual destructor.
     */
    bytes_impl::~bytes_impl()
    {
    }
    
    // runs when sample is received
    // useless for this block
    int bytes_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
    	return 0;
    }
    
    // runs when pdu is received
    // inserts bytes (preamble/access code) to beginning of pdu
    void bytes_impl::msg_handler(pmt::pmt_t pmt_msg)
    {
    	// convert received pdu to type that can be manipulated
    	std::vector<uint8_t> msg = pmt::u8vector_elements(pmt::cdr(pmt_msg));
    	std::vector<uint8_t> cut_msg = std::vector<uint8_t>(msg.begin(), msg.end());
    	
    	auto add = d_add;	// makes bytes to be added available in this function
    	while(add.length()){
    		// takes last 8 bits from add and puts them in a temporary string
    		std::string temp_add = add.substr(add.length()-8, 8);
    		add.erase(add.length()-8, add.length());
    		// counvert 8 bits from string to int
    		int int_add = std::stoi(temp_add);
    		// convert 8 bits from binary to decimal
    		int dec_value = 0;
    		int base = 1;
    		while (int_add) {
   			int last_digit = int_add % 10;
        		int_add = int_add / 10;
        		dec_value += last_digit * base;
        		base = base * 2;
    		}
    		// inserts decimal version of original string into beginning of pdu
    		// which can then convert from decimal to hexadecimal
    		// (cant do binary to hexadecimal which is why that must be done manually)
    		cut_msg.insert(cut_msg.begin(), dec_value);
    	}
    	
    	// outputs new pdu
    	message_port_pub(
        	pmt::mp("out"),
        	pmt::cons(pmt::car(pmt_msg), pmt::init_u8vector(cut_msg.size(), cut_msg)));
    }

  } /* namespace pduencode */
} /* namespace gr */
