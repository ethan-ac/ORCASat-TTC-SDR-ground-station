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

#ifndef INCLUDED_PDUENCODE_USRP_PAD_IMPL_H
#define INCLUDED_PDUENCODE_USRP_PAD_IMPL_H

#include <pduencode/usrp_pad.h>

namespace gr {
  namespace pduencode {

    class usrp_pad_impl : public usrp_pad
    {
     private:
      // parameters needed to calculate usrp padding needed
      int d_samples_per_symbol;
      int d_bits_per_symbol;


     public:
      usrp_pad_impl(int samples_per_symbol, int bits_per_symbol);
      ~usrp_pad_impl();

      // runs when called
      // calculates lowest common multiple of a and b
      int lcm(int a, int b);
      
      // runs when called
      // calculates # padding bytes needed for usrp to transmit
      int npadding_bytes(int pkt_byte_len, int samples_per_symbol, int bits_per_symbol);
      
      // runs when pdu is received
      // adds usrp padding to ending of pdu
      void msg_handler(pmt::pmt_t pmt_msg);

    };

  } // namespace pduencode
} // namespace gr

#endif /* INCLUDED_PDUENCODE_USRP_PAD_IMPL_H */

