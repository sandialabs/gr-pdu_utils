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

 #ifndef INCLUDED_PDU_UTILS_PDU_TO_BURSTS_C_H
 #define INCLUDED_PDU_UTILS_PDU_TO_BURSTS_C_H

#include <pdu_utils/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace pdu_utils {

    /*!
     * \brief Emit a PDU data as a stream.
     *
     * The PDU to Bursts block takes data stored in the uniform vector of a PDU
     * and emits it with UHD style tx_sob and tx_eob tags. If desired, the
     * metadata dictionary can contain a tx_time tag which will append a UHD
     * style tx_time tag to the tx_sob sample causing transmission at a well
     * defined point in time.
     *
     * \ingroup pdu_utils
     *
     */
    class PDU_UTILS_API pdu_to_bursts_c : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<pdu_to_bursts_c> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of pdu_utils::pdu_to_bursts_c.
       *
       * To avoid accidental use of raw pointers, pdu_utils::pdu_to_bursts_c's
       * constructor is in a private implementation
       * class. pdu_utils::pdu_to_bursts_c::make is the public interface for
       * creating new instances.
       */
      static sptr make(uint32_t early_burst_behavior, uint32_t max_queue_size);

      virtual void set_max_queue_size(uint32_t) = 0;

    };

  } // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_TO_BURSTS_C_H */
