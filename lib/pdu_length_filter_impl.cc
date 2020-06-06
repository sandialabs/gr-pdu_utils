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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "pdu_length_filter_impl.h"

namespace gr {
  namespace pdu_utils {

    pdu_length_filter::sptr
    pdu_length_filter::make(uint32_t length, bool drop_long)
    {
      return gnuradio::get_initial_sptr
        (new pdu_length_filter_impl(length, drop_long));
    }

    /*
     * The private constructor
     */
    pdu_length_filter_impl::pdu_length_filter_impl(uint32_t length, bool drop_long)
      : gr::block("pdu_length_filter",
              io_signature::make (0, 0, 0),
              io_signature::make (0, 0, 0)),
        d_length(length),
        d_drop_long(drop_long),
        d_n_passed(0),
        d_n_blocked(0)
  {
    message_port_register_in(PMTCONSTSTR__PDU_IN);
    set_msg_handler(PMTCONSTSTR__PDU_IN, boost::bind(&pdu_length_filter_impl::handle_pdu, this, _1));
    message_port_register_out(PMTCONSTSTR__PDU_OUT);
  }

    /*
     * Our virtual destructor.
     */
    pdu_length_filter_impl::~pdu_length_filter_impl()
    {
    }


    /*
     * This goes here as print statements in the destructor are usually not shown
     */
    bool
    pdu_length_filter_impl::stop()
    {
      GR_LOG_INFO(d_logger, boost::format("PDU length filter blocked %d and passed %d PDUs") % d_n_blocked % d_n_passed);
      return true;
    }


    /*
     * This goes here as print statements in the destructor are usually not shown
     */
     void
     pdu_length_filter_impl::handle_pdu(pmt::pmt_t pdu)
     {
       // make sure PDU data is formed properly
       if (!(pmt::is_pair(pdu))) {
         GR_LOG_WARN(d_logger, "received unexpected PMT (non-pair)");
         return;
       }

       if (!pmt::is_uniform_vector(pmt::cdr(pdu))) {
         GR_LOG_WARN(d_logger, "received unexpected PMT (CDR not uniform vector)");
         return;
       }

       size_t vlen = pmt::length(pmt::cdr(pdu));
       if ((d_drop_long && (vlen > d_length)) || (!d_drop_long && (vlen < d_length))) {
         d_n_blocked++;
         return;
       }
       d_n_passed++;
       message_port_pub(PMTCONSTSTR__PDU_OUT, pdu);
     }

  } /* namespace pdu_utils */
} /* namespace gr */
