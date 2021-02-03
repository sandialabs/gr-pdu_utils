/* -*- c++ -*- */
/*
 * Copyright 2021 gr-pdu_utils author.
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

#ifndef INCLUDED_PDU_UTILS_PDU_ROTATE_H
#define INCLUDED_PDU_UTILS_PDU_ROTATE_H

#include <pdu_utils/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace pdu_utils {

    /*!
     * \brief <+description of block+>
     * \ingroup pdu_utils
     *
     */
    class PDU_UTILS_API pdu_rotate : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<pdu_rotate> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of pdu_utils::pdu_rotate.
       *
       * @param phase_inc - phase increment in radians
       */
      static sptr make(double phase_inc);

      /**
       * Set Phase Increment
       *
       * @param phase_inc - Phase Increment
       */
      virtual void set_phase_inc(double phase_inc) = 0;

    };

  } // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_ROTATE_H */

