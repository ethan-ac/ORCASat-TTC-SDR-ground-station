/* -*- c++ -*- */
/*
 * Copyright 2021 gr-pduzmq author.
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

#ifndef INCLUDED_PDUZMQ_NEWZMQ_H
#define INCLUDED_PDUZMQ_NEWZMQ_H

#include <pduzmq/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace pduzmq {

    /*!
     * \brief <+description of block+>
     * \ingroup pduzmq
     *
     */
    class PDUZMQ_API newzmq : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<newzmq> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of pduzmq::newzmq.
       *
       * To avoid accidental use of raw pointers, pduzmq::newzmq's
       * constructor is in a private implementation
       * class. pduzmq::newzmq::make is the public interface for
       * creating new instances.
       */
      static sptr make(char* address, int timeout, bool bind);
    };

  } // namespace pduzmq
} // namespace gr

#endif /* INCLUDED_PDUZMQ_NEWZMQ_H */

