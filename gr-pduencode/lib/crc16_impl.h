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

#ifndef INCLUDED_PDUENCODE_CRC16_IMPL_H
#define INCLUDED_PDUENCODE_CRC16_IMPL_H

#include <pduencode/crc16.h>

namespace gr {
  namespace pduencode {

    class crc16_impl : public crc16
    {
     private:
      #define CRC16_POLY 0x8005;
      #define CRC_INIT 0xFFFF	// Init value for CRC calculation


     public:
      crc16_impl();
      ~crc16_impl();
      
      // runs when called
      // calculates 1 cycle of crc16 based on data/length fields of packet
      uint16_t culCalcCRC(unsigned char crcData, uint16_t crcReg);
      
      // runs when pdu is received
      // calculates and then adds crc16 to ending of pdu
      void msg_handler(pmt::pmt_t pmt_msg);
      
      // runs when sample is received
      // useless for this block
      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);

    };

  } // namespace pduencode
} // namespace gr

#endif /* INCLUDED_PDUENCODE_CRC16_IMPL_H */

