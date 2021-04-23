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
#include "byte_flip_impl.h"

namespace gr {
  namespace pduencode {

    byte_flip::sptr byte_flip::make(bool seqnum, bool hwid, bool crc16)
    {
      return gnuradio::get_initial_sptr (new byte_flip_impl(seqnum, hwid, crc16));
    }


    /*
     * The private constructor
     */
    byte_flip_impl::byte_flip_impl(bool seqnum, bool hwid, bool crc16)
      : gr::block("byte_flip",
              gr::io_signature::make(0, 0, 0),	// 0's so input can be of type pdu 
              gr::io_signature::make(0, 0, 0)),	// 0's so output can be of type pdu
              d_seqnum(seqnum),
              d_hwid(hwid),
              d_crc16(crc16)
    {
    	// sets up ports to be of type pdu
    	message_port_register_out(pmt::mp("out"));
    	message_port_register_in(pmt::mp("in"));
    	set_msg_handler(pmt::mp("in"), [this](pmt::pmt_t msg) { this->msg_handler(msg); });
    }

    /*
     * Our virtual destructor.
     */
    byte_flip_impl::~byte_flip_impl()
    {
    }

    // runs when pdu is received
    // flips the bytes of the seqnum, hwid, or crc16 bytes of a rf openlst packet if enabled and outputs
    void byte_flip_impl::msg_handler(pmt::pmt_t pmt_msg)
    {
    	// convert received pdu from pdu to std::vector<uint8_t>
    	std::vector<uint8_t> msg = pmt::u8vector_elements(pmt::cdr(pmt_msg));
    	
    	// calculates number of bytes in the data field
    	int data_offset = msg[0] - MIN_LENGTH;
    	
    	// flip seqnum bytes if enabled
    	if (d_seqnum) {
    		// error check if packet is large enough that seqnum is present to be flipped
    		if (4 > msg.size()) {
	    		std::cout << "===================================" << std::endl;
	    		std::cout << "ERROR Byte Flip: seqnum could not be flipped due to insufficient packet length" << std::endl;
	    		std::cout << "===================================" << std::endl;
    		}
    		
    		// flip seqnum bytes
		msg.insert(msg.begin()+4, msg[2]);
	    	msg.erase(msg.begin()+2);
    	}
    	
    	// flip hwid bytes if enabled
    	if (d_hwid) {
    		// error check if packet is large enough that hwid is present to be flipped
    		if (msg[0]-1 > msg.size()) {
	    		std::cout << "===================================" << std::endl;
	    		std::cout << "ERROR Byte Flip: hwid could not be flipped due to insufficient packet length" << std::endl;
	    		std::cout << "===================================" << std::endl;
    		}
    		
    		// flip hwid bytes
	    	msg.insert(msg.begin()+8+data_offset, msg[6+data_offset]);
		msg.erase(msg.begin()+6+data_offset);
    	}
    	
    	// flip crc16 bytes if enabled
    	if (d_crc16){
    		// error check if packet is large enough that crc16 is present to be flipped
		if (msg[0]+1 > msg.size()) {
	    		std::cout << "===================================" << std::endl;
	    		std::cout << "ERROR Byte Flip: crc16 could not be flipped due to insufficient packet length" << std::endl;
	    		std::cout << "===================================" << std::endl;
    		}
    		
    		// flip crc16 bytes
		msg.insert(msg.begin()+10+data_offset, msg[8+data_offset]);
		msg.erase(msg.begin()+8+data_offset);
	}
    	
    	// outputs new pdu
    	message_port_pub(
        	pmt::mp("out"),
        	pmt::cons(pmt::car(pmt_msg), pmt::init_u8vector(msg.size(), msg)));
    }

  } /* namespace pduencode */
} /* namespace gr */

