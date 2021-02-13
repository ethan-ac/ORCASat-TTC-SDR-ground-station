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

#ifndef INCLUDED_PDUENCODE_BYTES_IMPL_H
#define INCLUDED_PDUENCODE_BYTES_IMPL_H

#include <pduencode/bytes.h>

namespace gr {
  namespace pduencode {

    class bytes_impl : public bytes
    {
     private:
      // string to be added to pdu
      std::string d_add;


     public:
      bytes_impl(std::string add);
      ~bytes_impl();

      // runs when pdu is received
      // add bytes (preamble/access code) to beginning of pdu
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

#endif /* INCLUDED_PDUENCODE_BYTES_IMPL_H */

