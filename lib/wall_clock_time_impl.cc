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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "wall_clock_time_impl.h"

namespace gr {
  namespace pdu_utils {

    wall_clock_time::sptr
    wall_clock_time::make()
    {
      return gnuradio::get_initial_sptr
        (new wall_clock_time_impl());
    }

    /*
     * The private constructor
     */
    wall_clock_time_impl::wall_clock_time_impl()
      : gr::block("wall_clock_time",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0))
    {
      boost::posix_time::ptime epoch(boost::gregorian::date(1970,1,1));
      d_epoch = epoch;

      message_port_register_in(PMTCONSTSTR__PDU_IN);
      set_msg_handler(PMTCONSTSTR__PDU_IN,
          boost::bind(&wall_clock_time_impl::handle_pdu, this, _1));
      message_port_register_out(PMTCONSTSTR__PDU_OUT);
    }

    /*
     * Our virtual destructor.
     */
    wall_clock_time_impl::~wall_clock_time_impl()
    {
    }

    void
    wall_clock_time_impl::handle_pdu(pmt::pmt_t pdu)
    {
      // make sure PDU data is formed properly
      if (!(pmt::is_pair(pdu))) {
        GR_LOG_WARN(d_logger, "received unexpected PMT (non-pair)");
        return;
      }

      pmt::pmt_t meta = pmt::car(pdu);
      pmt::pmt_t v_data = pmt::cdr(pdu);

      // append time and publish
      double t_now((boost::get_system_time() - d_epoch).total_microseconds()/1000000.0);
      meta = pmt::dict_add(meta, PMTCONSTSTR__WALL_CLOCK_TIME, pmt::from_double(t_now));
      message_port_pub(PMTCONSTSTR__PDU_OUT, (pmt::cons(meta, v_data)));
    }

  } /* namespace pdu_utils */
} /* namespace gr */

