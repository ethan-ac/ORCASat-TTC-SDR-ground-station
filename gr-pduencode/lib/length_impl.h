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

#ifndef INCLUDED_PDUENCODE_LENGTH_IMPL_H
#define INCLUDED_PDUENCODE_LENGTH_IMPL_H

#include <pduencode/length.h>

namespace gr {
  namespace pduencode {

    class length_impl : public length
    {
     private:
      // Nothing to declare in this block.


     public:
      length_impl();
      ~length_impl();

      // runs when pdu is received
      // calculates then adds length field to beginning of pdu
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

#endif /* INCLUDED_PDUENCODE_LENGTH_IMPL_H */

