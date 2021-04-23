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
#include "syncword_impl.h"

namespace gr {
  namespace pduencode {

    syncword::sptr syncword::make(std::string syncword_str)
    {
      return gnuradio::get_initial_sptr (new syncword_impl(syncword_str));
    }


    /*
     * The private constructor
     */
    syncword_impl::syncword_impl(std::string syncword_str)
      : gr::block("syncword",
              gr::io_signature::make(0, 0, 0),	// 0's so input can be of type pdu 
              gr::io_signature::make(0, 0, 0)),	// 0's so output can be of type pdu
              d_syncword_str(syncword_str)	// string to be added to pdu
    {
    	// remove all spaces from d_value
    	while (d_syncword_str.find(" ") < d_syncword_str.size()) {
    		std::size_t index = d_syncword_str.find(" ");
    		d_syncword_str.erase(d_syncword_str.begin()+index);
    	}
    	
    	// error check if syncword bits given are a valid length
    	if (d_syncword_str.size() > 32) {
    		std::cout << "===================================" << std::endl;
    		std::cout << "ERROR Add Syncword: syncword input is larger than expected, the hex value must be 32 bits in length" << std::endl;
    		std::cout << "===================================" << std::endl;
    	} else if (d_syncword_str.size() < 32) {
    		std::cout << "===================================" << std::endl;
    		std::cout << "ERROR Add Syncword: syncword input is smaller than expected, the hex value must be 32 bits in length" << std::endl;
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
    syncword_impl::~syncword_impl()
    {
    }

    // runs when pdu is received
    // add syncword bits to head of pdu and outputs
    void syncword_impl::msg_handler(pmt::pmt_t pmt_msg)
    {
    	// convert received pdu from pdu to std::vector<uint8_t>
    	std::vector<uint8_t> msg = pmt::u8vector_elements(pmt::cdr(pmt_msg));
    	
    	// make temporary d_syncword_str so it can be erased from without negative effects
    	std::string syncword_temp = d_syncword_str;
    	
    	// runs while syncword_temp has values in it
    	// inserts syncword bits into pdu at head
    	while(syncword_temp.length()){
    		// takes last 8 bits from syncword_temp and puts them in another temporary string
    		std::string syncword_bits = syncword_temp.substr(syncword_temp.length()-8, 8);
    		
    		// converts 8 bits from std::string to uint8_t byte
    		uint8_t syncword_hex = std::stoi(syncword_bits, nullptr, 2);
    		
    		// add uint8_t byte to head of pdu
    		msg.insert(msg.begin(), syncword_hex);
    		
    		// erase value thats already been added to pdu so it wont be added again
    		syncword_temp.erase(syncword_temp.length()-8, syncword_temp.length());
    	}
    	
    	// outputs new pdu
    	message_port_pub(
        	pmt::mp("out"),
        	pmt::cons(pmt::car(pmt_msg), pmt::init_u8vector(msg.size(), msg)));
    }

  } /* namespace pduencode */
} /* namespace gr */

