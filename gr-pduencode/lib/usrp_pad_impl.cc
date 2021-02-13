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
#include "usrp_pad_impl.h"
#include <algorithm>
#include <vector>
#include <string>
#include <cmath>

namespace gr {
  namespace pduencode {

    usrp_pad::sptr usrp_pad::make(int samples_per_symbol, int bits_per_symbol)
    {
      return gnuradio::get_initial_sptr (new usrp_pad_impl(samples_per_symbol, bits_per_symbol));
    }


    /*
     * The private constructor
     */
    usrp_pad_impl::usrp_pad_impl(int samples_per_symbol, int bits_per_symbol)
      : gr::block("usrp_pad",
              gr::io_signature::make(0, 0, 0),	// 0's so input can be of type pdu 
              gr::io_signature::make(0, 0, 0)),	// 0's so output can be of type pdu
              // parameters needed to calculate usrp padding needed
              d_samples_per_symbol(samples_per_symbol),
              d_bits_per_symbol(bits_per_symbol)
    {
    	// sets up ports to be of type pdu
    	message_port_register_out(pmt::mp("out"));
    	message_port_register_in(pmt::mp("in"));
    	set_msg_handler(pmt::mp("in"), [this](pmt::pmt_t msg) { this->msg_handler(msg); });
    }

    /*
     * Our virtual destructor.
     */
    usrp_pad_impl::~usrp_pad_impl()
    {
    }
    
    // runs when sample is received
    // useless for this block
    int usrp_pad_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      return 0;
    }
    
    // runs when called
    // calculates lowest common multiple of a and b
    int usrp_pad_impl::lcm(int a, int b)
    {
    	int lcm;
    	if(a>b) {
        	lcm = a;
        }
    	else {
        	lcm = b;
        }
    	while(1) {
        	if( lcm%a==0 && lcm%b==0 )
            	return lcm;
        	lcm++;
        }
    }
    
    // runs when called
    // calculates # padding bytes needed for usrp to transmit
    int usrp_pad_impl::npadding_bytes(int pkt_byte_len, int samples_per_symbol, int bits_per_symbol)
    {
    	int modulus = 128;
    	int byte_modulus = floor((lcm(floor(modulus/8), samples_per_symbol) * bits_per_symbol) / samples_per_symbol);
    	int r = pkt_byte_len % byte_modulus;
    	if(r == 0) {
        	return 0;
        }
    	return byte_modulus - r;
    }
    
    // runs when pdu is received
    // adds usrp padding to ending of pdu
    void usrp_pad_impl::msg_handler(pmt::pmt_t pmt_msg)
    {
    	// convert received pdu to type that can be manipulated
    	std::vector<uint8_t> msg = pmt::u8vector_elements(pmt::cdr(pmt_msg));
    	std::vector<uint8_t> cut_msg = std::vector<uint8_t>(msg.begin(), msg.end());
    	
    	// makes these variables available in this function
    	auto samples_per_symbol = d_samples_per_symbol;
    	auto bits_per_symbol = d_bits_per_symbol;
    	// calculates # padding bytes needed for usrp to transmit
    	int num_bytes = npadding_bytes(cut_msg.size(), d_samples_per_symbol, d_bits_per_symbol);
    	// inserts usrp padding bytes at ending of pdu
    	for(int i = 0; i < num_bytes; i++) {
        	cut_msg.push_back(0x55);
        }
    
    	// outputs new pdu
    	message_port_pub(
        	pmt::mp("out"),
        	pmt::cons(pmt::car(pmt_msg), pmt::init_u8vector(cut_msg.size(), cut_msg)));
    }

  } /* namespace pduencode */
} /* namespace gr */

