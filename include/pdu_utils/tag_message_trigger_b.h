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

#ifndef INCLUDED_PDU_UTILS_TAG_MESSAGE_TRIGGER_B_H
#define INCLUDED_PDU_UTILS_TAG_MESSAGE_TRIGGER_B_H

#include <pdu_utils/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace pdu_utils {

    /*!
     * \brief <+description of block+>
     * \ingroup pdu_utils
     *
     */
    class PDU_UTILS_API tag_message_trigger_b : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<tag_message_trigger_b> sptr;

      static sptr make(pmt::pmt_t trigger_key, pmt::pmt_t arming_key, pmt::pmt_t message, uint32_t holdoff, double samp_rate=0, double start_time=0, double delay_time=0, bool tpdu_mode=false);

      virtual void set_trigger_key(pmt::pmt_t) = 0;
      virtual void set_arming_key(pmt::pmt_t) = 0;
      virtual void set_time_tag_key(pmt::pmt_t) = 0;
      virtual void set_message(pmt::pmt_t) = 0;
      virtual void set_holdoff(uint32_t) = 0;
      virtual void set_samp_rate(double) = 0;
      virtual void set_start_time(double) = 0;
      virtual void set_delay_time(double) = 0;
      virtual void set_tx_limit(uint64_t) = 0;
    };

  } // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_TAG_MESSAGE_TRIGGER_B_H */
