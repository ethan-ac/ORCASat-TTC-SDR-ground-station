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

#ifndef INCLUDED_PDUENCODE_BYTE_FLIP_IMPL_H
#define INCLUDED_PDUENCODE_BYTE_FLIP_IMPL_H

#include <pduencode/byte_flip.h>

// minimum length of an openlst rf packet with no data in it
#define MIN_LENGTH 9

namespace gr {
  namespace pduencode {

    class byte_flip_impl : public byte_flip
    {
     private:
      // determines if seqnum bytes should be flipped
      bool d_seqnum;
      
      // determines if hwid bytes should be flipped
      bool d_hwid;
      
      // determines if crc16 bytes should be flipped
      bool d_crc16;

     public:
      byte_flip_impl(bool seqnum, bool hwid, bool crc16);
      ~byte_flip_impl();

      // runs when pdu is received
      // flips the bytes of the seqnum, hwid, or crc16 bytes of a openlst rf packet if enabled and outputs
      void msg_handler(pmt::pmt_t pmt_msg);

    };

  } // namespace pduencode
} // namespace gr

#endif /* INCLUDED_PDUENCODE_BYTE_FLIP_IMPL_H */

