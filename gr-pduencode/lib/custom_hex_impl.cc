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
#include "custom_hex_impl.h"

namespace gr {
  namespace pduencode {

    custom_hex::sptr custom_hex::make(bool head_tail, std::string hex_str)
    {
      return gnuradio::get_initial_sptr (new custom_hex_impl(head_tail, hex_str));
    }
    

    /*
     * The private constructor
     */
    custom_hex_impl::custom_hex_impl(bool head_tail, std::string hex_str)
      : gr::block("custom_hex",
              gr::io_signature::make(0, 0, 0),	// 0's so input can be of type pdu 
              gr::io_signature::make(0, 0, 0)),	// 0's so output can be of type pdu
              d_head_tail(head_tail),
              d_hex_str(hex_str)
    {
    	// remove all spaces from d_value
    	while (d_hex_str.find(" ") < d_hex_str.size()) {
    		std::size_t index = d_hex_str.find(" ");
    		d_hex_str.erase(d_hex_str.begin()+index);
    	}
    	
    	// error check if hex value given is an invalid length
    	if (d_hex_str.size() > 4) {
    		std::cout << "===================================" << std::endl;
    		std::cout << "ERROR Add Hex Values: hex input is larger than expected, the hex value must be 2/4 hex values or 1/2 bytes in length" << std::endl;
    		std::cout << "===================================" << std::endl;
    	} else if (d_hex_str.size() == 3) {
    		std::cout << "===================================" << std::endl;
    		std::cout << "ERROR Add Hex Values: hex input is between expected sizes, the hex value must be 2/4 hex values or 1/2 bytes in length" << std::endl;
    		std::cout << "===================================" << std::endl;
    	} else if (d_hex_str.size() < 2) {
    		std::cout << "===================================" << std::endl;
    		std::cout << "ERROR Add Hex Values: hex input is smaller than expected, the hex value must be 2/4 hex values or 1/2 bytes in length" << std::endl;
    		std::cout << "===================================" << std::endl;
    	}
    	
    	// sets up ports to be of type pdu
    	message_port_register_out(pmt::mp("out"));
    	message_port_register_in(pmt::mp("in"));
    	set_msg_handler(pmt::mp("in"), [this](pmt::pmt_t msg) { this->msg_handler(msg); });
    }

    /*
     * Our virtual destructor.
     */
    custom_hex_impl::~custom_hex_impl()
    {
    }

    // runs when pdu is received
    // add hex values into pdu at head or tail and outputs
    void custom_hex_impl::msg_handler(pmt::pmt_t pmt_msg)
    {
    	// convert received pdu from pdu to std::vector<uint8_t>
    	std::vector<uint8_t> msg = pmt::u8vector_elements(pmt::cdr(pmt_msg));
    	
    	// make temporary d_hex_str so can erase from it without negative effect
    	std::string hex_temp = d_hex_str;
    	
    	// runs while hex_temp has values in it
    	// add hex values of type std::string into pdu at head or tail
    	while(!hex_temp.empty()){
		// add hex byte to head if true, tail if false
		if (d_head_tail) {
			// take first 2 values of hex_temp
			// this is a std::string representation of hex byte
			std::string hex_byte = hex_temp.substr(hex_temp.size()-2, 2);
			
			// convert std::string representation of hex byte to actual uint8_t hex byte
			uint8_t hex_hex = std::stoi(hex_byte, nullptr, 16);
			
			// add hex byte to head of pdu
			msg.insert(msg.begin(), hex_hex);
			
			// erase value thats already been added to pdu so it wont be added again
			hex_temp.erase(hex_temp.end()-2, hex_temp.end());
		} else { 
		    	// take first 2 values of hex_temp
			// this is a std::string representation of hex byte
			std::string hex_byte = hex_temp.substr(0,2);
			
			// convert std::string representation of hex byte to actual uint8_t hex byte
			uint8_t hex_hex = std::stoi(hex_byte, nullptr, 16);
		    	
			// add hex byte to tail of pdu
		    	msg.push_back(hex_hex);
		    	
			// erase value thats already been added to pdu so it wont be added again
			hex_temp.erase(hex_temp.begin(), hex_temp.begin()+2);
		}
	}
	
    	// outputs new pdu
    	message_port_pub(
        	pmt::mp("out"),
        	pmt::cons(pmt::car(pmt_msg), pmt::init_u8vector(msg.size(), msg)));
    }

  } /* namespace pduencode */
} /* namespace gr */
