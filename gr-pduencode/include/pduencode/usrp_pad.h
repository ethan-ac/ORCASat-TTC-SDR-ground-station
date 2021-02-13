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

#ifndef INCLUDED_PDUENCODE_USRP_PAD_H
#define INCLUDED_PDUENCODE_USRP_PAD_H

#include <pduencode/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace pduencode {

    /*!
     * \brief <+description of block+>
     * \ingroup pduencode
     *
     */
    class PDUENCODE_API usrp_pad : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<usrp_pad> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of pduencode::usrp_pad.
       *
       * To avoid accidental use of raw pointers, pduencode::usrp_pad's
       * constructor is in a private implementation
       * class. pduencode::usrp_pad::make is the public interface for
       * creating new instances.
       */
      static sptr make(int samples_per_symbol, int bits_per_symbol);
    };

  } // namespace pduencode
} // namespace gr

#endif /* INCLUDED_PDUENCODE_USRP_PAD_H */

