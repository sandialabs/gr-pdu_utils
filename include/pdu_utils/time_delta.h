/* -*- c++ -*- */
/*
 * Copyright 2018 <+YOU OR YOUR COMPANY+>.
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


#ifndef INCLUDED_PDU_UTILS_TIME_DELTA_H
#define INCLUDED_PDU_UTILS_TIME_DELTA_H

#include <pdu_utils/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace pdu_utils {

    /*!
     * \brief <+description of block+>
     * \ingroup pdu_utils
     *
     */
    class PDU_UTILS_API time_delta : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<time_delta> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of pdu_utils::time_delta.
       *
       * To avoid accidental use of raw pointers, pdu_utils::time_delta's
       * constructor is in a private implementation
       * class. pdu_utils::time_delta::make is the public interface for
       * creating new instances.
       */
      static sptr make(const std::string &name);
    };

  } // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_TIME_DELTA_H */
