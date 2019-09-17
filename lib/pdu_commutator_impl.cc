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
#include "pdu_commutator_impl.h"

namespace gr {
  namespace pdu_utils {

    pdu_commutator::sptr
    pdu_commutator::make(size_t num_paths)
    {
      return gnuradio::get_initial_sptr
        (new pdu_commutator_impl(num_paths));
    }

    /*
     * The private constructor
     */
    pdu_commutator_impl::pdu_commutator_impl(size_t num_paths)
      : gr::block("pdu_commutator",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0)),
        d_num_paths(num_paths)
    {
      d_current_path = 0;
      message_port_register_in(PMTCONSTSTR__PDU_IN);
      set_msg_handler(PMTCONSTSTR__PDU_IN,
          boost::bind(&pdu_commutator_impl::handle_pdu, this, _1));
      for (size_t i = 0; i < d_num_paths; i++) {
        std::stringstream s;
        s << "pdu_out_" << i;
        d_output_ports.push_back(pmt::mp(s.str()));
        message_port_register_out(d_output_ports[i]);
      }
    }

    /*
     * Our virtual destructor.
     */
    pdu_commutator_impl::~pdu_commutator_impl()
    {
    }

    void pdu_commutator_impl::handle_pdu(pmt::pmt_t pdu) {
      message_port_pub(d_output_ports[d_current_path], pdu);
      d_current_path++;
      if (d_current_path == d_num_paths) d_current_path = 0;
    }


  } /* namespace pdu_utils */
} /* namespace gr */

