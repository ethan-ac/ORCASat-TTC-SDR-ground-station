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
#include "autoswitch_impl.h"

namespace gr {
  namespace pduencode {

    autoswitch::sptr autoswitch::make()
    {
      return gnuradio::get_initial_sptr (new autoswitch_impl());
    }


    /*
     * The private constructor
     */
    autoswitch_impl::autoswitch_impl()
      : gr::block("autoswitch",
              gr::io_signature::make(0, 0, 0),	// 0's so input can be of type pdu 
              gr::io_signature::make(0, 0, 0))	// 0's so output can be of type pdu
    {
    	// sets up ports to be of type pdu
    	message_port_register_out(pmt::mp("CC1110"));
    	message_port_register_out(pmt::mp("OpenLST"));
    	message_port_register_in(pmt::mp("in"));
    	set_msg_handler(pmt::mp("in"), [this](pmt::pmt_t msg) { this->msg_handler(msg); });
    }

    /*
     * Our virtual destructor.
     */
    autoswitch_impl::~autoswitch_impl()
    {
    }
    
    // runs when called
    // calculates 1 cycle of crc16 based on data/length fields of packet
    // based on code example found here: https://www.ti.com/lit/an/swra111e/swra111e.pdf?	ts=1613001791620&ref_url=https%253A%252F%252Fwww.google.com%252F
    uint16_t autoswitch_impl::culCalcCRC(unsigned char crcData, uint16_t crcReg)
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
    // outputs to CC1110 port if message originated from CC1110, outputs to openlst port if message originated from board running openlst
    // NOTE: to simplify, everythin between the length field and crc16 of a packet from openlst will be considered to be the data field
    // packet from CC1110: 
    //     cut_msg:                     04 66 64 73 61
    //                                  length(04) data(66 64 73 61)
    //     crc16_cut_msg:               04 66 64 73 61 f1 62
    //                                  length(04) data(66 64 73 61) crc16(f1 62)
    //     cut_msg w/ calculated crc16: 04 66 64 73 61 f1 62
    //                                  length(04) data(66 64 73 61) calculated crc16(f1 62)
    // packet from openlst:
    //     cut_msg:                     0d 40 6b ef 01 11 66 64 73 61 00 02 83 0a
    //                                  length(0d) data(40 6b ef 01 11 66 64 73 61 00 02) actual crc16(83 0a)
    //     crc16_cut_msg:               0d 40 6b ef 01 11 66 64 73 61 00 02 83 0a 99 33
    //                                  length(0d) data(40 6b ef 01 11 66 64 73 61 00 02) actual crc16(83 0a) noise(99 33)
    //     cut msg w/ calculated crc16: 0d 40 6b ef 01 11 66 64 73 61 00 02 83 0a e3 2a
    //                                  length(0d) data(40 6b ef 01 11 66 64 73 61 00 02) actual crc16(83 0a) calculated crc16(e3 2a)
    // when packet is sent from CC1110 crc16_cut_msg == cut_msg + calculated crc16
    // but when packet is sent from openlst crc16_cut_msg != cut_msg + calculated crc16
    // this fact is used to automatically determine if a packet originated from CC1110 or openlst
    void autoswitch_impl::msg_handler(pmt::pmt_t pmt_msg)
    {
    	// convert received pdu from pdu to std::vector<uint8_t>
    	std::vector<uint8_t> msg = pmt::u8vector_elements(pmt::cdr(pmt_msg));
    	
    	// reads length field of packet and converts to int
    	// +1 so length field is included in cut_msg
    	int packet_len = std::stoi(std::to_string(msg[0])) + 1;
    	// puts length and data field of packet into uint8_t vector
    	std::vector<uint8_t> cut_msg = std::vector<uint8_t>(msg.begin(), msg.begin() + packet_len);
    	
    	// reads length field of packet and converts to int
    	// +3 so length field and crc16 are included in cut_msg
    	int packet_crc16_len = std::stoi(std::to_string(msg[0])) + 3;
    	// puts length field, data field, and crc16 of packet into uint8_t vector
    	std::vector<uint8_t> crc16_cut_msg = std::vector<uint8_t>(msg.begin(), msg.begin() + packet_crc16_len);

    	// iteratively calculates crc16
    	// more code from link found before crc16_impl::culCalcCRC function
    	uint16_t checksum;
    	uint8_t i;
    	checksum = CRC_INIT; // Init value for CRC calculation
    	for (i = 0; i < cut_msg.size(); i++) {
        	checksum = culCalcCRC(msg[i], checksum);
        }
        
    	// inserts calculated crc16 into tail of cut_msg
    	for (int i = sizeof(checksum)-1; i >= 0; i--) {
        	cut_msg.push_back((checksum >> i*8) & 0xff);
        }
    	
    	// checks if cut_msg is same as crc16_cut_msg, if so message originated from zmq
    	if(cut_msg == crc16_cut_msg) {
    		std::cout << "===================================" << std::endl;
    		std::cout << "CC1110 structured packet" << std::endl;
    		std::cout << "===================================" << std::endl;
    		
	    	// outputs new pdu
	    	message_port_pub(
			pmt::mp("CC1110"),
			pmt::cons(pmt::car(pmt_msg), pmt::init_u8vector(msg.size(), msg)));
        } else {
	    	std::cout << "===================================" << std::endl;
	    	std::cout << "OPENLST structured RF packet" << std::endl;
	    	std::cout << "===================================" << std::endl;
	    	
	    	// outputs new pdu
	    	message_port_pub(
			pmt::mp("OpenLST"),
			pmt::cons(pmt::car(pmt_msg), pmt::init_u8vector(cut_msg.size(), cut_msg)));
        }
    }

  } /* namespace pduencode */
} /* namespace gr */

