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


 /* WARNING: this file is machine generated. Edits will be overwritten */

 #ifndef INCLUDED_PDU_UTILS_TAKE_SKIP_TO_PDU_F_H
 #define INCLUDED_PDU_UTILS_TAKE_SKIP_TO_PDU_F_H

#include <pdu_utils/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace pdu_utils {

    #ifndef MAXIMUM_PDU_SIZE
    #define MAXIMUM_PDU_SIZE
    static const uint32_t TAKESKIP_MAXIMUM_PDU_SIZE = 8192;
    #endif

    /*!
     * \brief <+description of block+>
     * \ingroup pdu_utils
     *
     */
    class PDU_UTILS_API take_skip_to_pdu_f : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<take_skip_to_pdu_f> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of pdu_utils::take_skip_to_pdu.
       *
       * To avoid accidental use of raw pointers, pdu_utils::take_skip_to_pdu's
       * constructor is in a private implementation
       * class. pdu_utils::take_skip_to_pdu::make is the public interface for
       * creating new instances.
       */
      virtual void set_take(uint32_t take) = 0;
      virtual void set_skip(uint32_t skip) = 0;

      static sptr make(uint32_t take, uint32_t skip);
    };

  } // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_TAKE_SKIP_TO_PDU_H */
