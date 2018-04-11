/* -*- c++ -*- */
/*
 * Copyright 2018 National Technology & Engineering Solutions of Sandia, LLC (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government retains certain rights in this software.
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


#ifndef INCLUDED_PDU_UTILS_PDU_PREAMBLE_H
#define INCLUDED_PDU_UTILS_PDU_PREAMBLE_H

#include <pdu_utils/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace pdu_utils {

    /*!
     * \brief <+description of block+>
     * \ingroup pdu_utils
     *
     */
    class PDU_UTILS_API pdu_preamble : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<pdu_preamble> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of pdu_utils::pdu_preamble.
       *
       * To avoid accidental use of raw pointers, pdu_utils::pdu_preamble's
       * constructor is in a private implementation
       * class. pdu_utils::pdu_preamble::make is the public interface for
       * creating new instances.
       */
      static sptr make(const std::vector<uint8_t>, const std::vector<uint8_t>, uint32_t interp=1, uint32_t zero_pad=0, bool nrz=true);
      virtual void set_preamble(const std::vector<uint8_t>) = 0;
      virtual void set_tail(const std::vector<uint8_t>) = 0;
      virtual void set_interp(uint32_t) = 0;
      virtual void set_zero_pad(uint32_t) = 0;
      virtual void set_nrz(bool) = 0;
    };

  } // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_PREAMBLE_H */
