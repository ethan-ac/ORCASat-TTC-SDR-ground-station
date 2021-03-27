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

#ifndef INCLUDED_PDUENCODE_SYNCWORD_IMPL_H
#define INCLUDED_PDUENCODE_SYNCWORD_IMPL_H

#include <pduencode/syncword.h>

namespace gr {
  namespace pduencode {

    class syncword_impl : public syncword
    {
     private:
      // bits which will be inserted at the head of a pdu as the syncword
      std::string d_syncword_str;

     public:
      syncword_impl(std::string syncword_str);
      ~syncword_impl();

      // runs when pdu is received
      // add syncword bits to head of pdu and outputs
      void msg_handler(pmt::pmt_t pmt_msg);

    };

  } // namespace pduencode
} // namespace gr

#endif /* INCLUDED_PDUENCODE_SYNCWORD_IMPL_H */

