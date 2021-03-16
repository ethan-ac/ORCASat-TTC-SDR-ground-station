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
#include "crc16_impl.h"
#include <algorithm>
#include <vector>
#include <string>

namespace gr {
  namespace pduencode {

    crc16::sptr crc16::make()
    {
      return gnuradio::get_initial_sptr (new crc16_impl());
    }


    /*
     * The private constructor
     */
    crc16_impl::crc16_impl()
      : gr::block("crc16",
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
    crc16_impl::~crc16_impl()
    {
    }
    
    // runs when called
    // calculates 1 cycle of crc16 based on data/length fields of packet
    // based on code example found here: https://www.ti.com/lit/an/swra111e/swra111e.pdf?ts=1613001791620&ref_url=https%253A%252F%252Fwww.google.com%252F
    uint16_t crc16_impl::culCalcCRC(unsigned char crcData, uint16_t crcReg)
    {
        uint8_t i;
    	for (i = 0; i < 8; i++) {
        	if (((crcReg & 0x8000) >> 8) ^ (crcData & 0x80)) {
            		crcReg = (crcReg << 1) ^ CRC16_POLY;
            	}
        	else {
            		crcReg = (crcReg << 1);
            	}
        	crcData <<= 1;
        }
    	return crcReg;
    }
    
    // runs when pdu is received
    // calculates and then adds crc16 to end of pdu
    void crc16_impl::msg_handler(pmt::pmt_t pmt_msg)
    {
    	// convert received pdu to type that can be manipulated
    	std::vector<uint8_t> msg = pmt::u8vector_elements(pmt::cdr(pmt_msg));
    	std::vector<uint8_t> cut_msg = std::vector<uint8_t>(msg.begin(), msg.end());
    	
    	// iteratively calculates crc16
    	// more code from link found before crc16_impl::culCalcCRC function
    	uint16_t checksum;
    	uint8_t i;
    	checksum = CRC_INIT; // Init value for CRC calculation
    	for (i = 0; i < cut_msg.size(); i++) {
        	checksum = culCalcCRC(cut_msg[i], checksum);
        }
        // inserts crc16 into ending of pdu
    	for (int i = sizeof(checksum)-1; i >= 0; i--) {
        	cut_msg.push_back((checksum >> i*8) & 0xff);
        }
        
        // outputs new pdu
    	message_port_pub(
        	pmt::mp("out"),
        	pmt::cons(pmt::car(pmt_msg), pmt::init_u8vector(cut_msg.size(), cut_msg)));
    }

  } /* namespace pduencode */
} /* namespace gr */

