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

#ifndef INCLUDED_PDUENCODE_SELECTOR_TEST_IMPL_H
#define INCLUDED_PDUENCODE_SELECTOR_TEST_IMPL_H

#include <pduencode/selector_test.h>

namespace gr {
  namespace pduencode {

    class selector_test_impl : public selector_test
    {
     private:
      // determines which port pdu is outputted from
      bool d_state;

     public:
      selector_test_impl(bool state);
      ~selector_test_impl();

      // runs when pdu is received
      // outputs pdu to specified port
      void msg_handler(pmt::pmt_t pmt_msg);

    };

  } // namespace pduencode
} // namespace gr

#endif /* INCLUDED_PDUENCODE_SELECTOR_TEST_IMPL_H */

