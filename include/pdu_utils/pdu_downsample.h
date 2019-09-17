/* -*- c++ -*- */
/* 
 * Copyright 2019 gr-pdu_utils author.
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


#ifndef INCLUDED_PDU_UTILS_PDU_DOWNSAMPLE_H
#define INCLUDED_PDU_UTILS_PDU_DOWNSAMPLE_H

#include <pdu_utils/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace pdu_utils {

    /*!
     * \brief <+description of block+>
     * \ingroup pdu_utils
     *
     */
    class PDU_UTILS_API pdu_downsample : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<pdu_downsample> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of pdu_utils::pdu_downsample.
       *
       * To avoid accidental use of raw pointers, pdu_utils::pdu_downsample's
       * constructor is in a private implementation
       * class. pdu_utils::pdu_downsample::make is the public interface for
       * creating new instances.
       */
      static sptr make(int decimation, int phase);
    };

  } // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_DOWNSAMPLE_H */

