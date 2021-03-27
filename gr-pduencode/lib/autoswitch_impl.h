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

#ifndef INCLUDED_PDUENCODE_AUTOSWITCH_IMPL_H
#define INCLUDED_PDUENCODE_AUTOSWITCH_IMPL_H

#include <pduencode/autoswitch.h>

// constants for crc16 calculation
#define CRC16_POLY 0x8005;
#define CRC_INIT 0xFFFF

namespace gr {
  namespace pduencode {

    class autoswitch_impl : public autoswitch
    {
     private:

     public:
      autoswitch_impl();
      ~autoswitch_impl();
      
      // runs when called
      // calculates 1 cycle of crc16 based on current fields of packet
      uint16_t culCalcCRC(unsigned char crcData, uint16_t crcReg);
      
      // runs when pdu is received
      // outputs to zmq port if message originated from zmq, outputs to openlst port if message originated from board running openlst
      void msg_handler(pmt::pmt_t pmt_msg);

    };

  } // namespace pduencode
} // namespace gr

#endif /* INCLUDED_PDUENCODE_AUTOSWITCH_IMPL_H */

