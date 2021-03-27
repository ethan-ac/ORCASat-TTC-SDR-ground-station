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

#ifndef INCLUDED_PDUENCODE_CUSTOM_HEX_IMPL_H
#define INCLUDED_PDUENCODE_CUSTOM_HEX_IMPL_H

#include <pduencode/custom_hex.h>

namespace gr {
  namespace pduencode {

    class custom_hex_impl : public custom_hex
    {
     private:
      // hex value to be inserted at pdu at head or tail
      std::string d_hex_str;
      
      // determines if hex value will be inserted at pdu head or tail
      bool d_head_tail;
      
     public:
      custom_hex_impl(bool head_tail, std::string hex_str);
      ~custom_hex_impl();

      // runs when pdu is received
      // add hex values into pdu at head or tail and outputs
      void msg_handler(pmt::pmt_t pmt_msg);

    };

  } // namespace pduencode
} // namespace gr

#endif /* INCLUDED_PDUENCODE_CUSTOM_HEX_IMPL_H */

