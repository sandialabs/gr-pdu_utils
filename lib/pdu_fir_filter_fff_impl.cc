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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "pdu_fir_filter_fff_impl.h"

namespace gr {
  namespace pdu_utils {

    pdu_fir_filter_fff::sptr
    pdu_fir_filter_fff::make(const std::vector<float> taps)
    {
      return gnuradio::get_initial_sptr
        (new pdu_fir_filter_fff_impl(taps));
    }

    /*
     * The private constructor
     */
    pdu_fir_filter_fff_impl::pdu_fir_filter_fff_impl(const std::vector<float> taps)
      : gr::block("pdu_fir_filter_fff",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0)),
        d_taps(taps)
    {
      d_fir = new filter::kernel::fir_filter_fff(1, taps);

      message_port_register_in(PMTCONSTSTR__PDU_IN);
      set_msg_handler(PMTCONSTSTR__PDU_IN,
          boost::bind(&pdu_fir_filter_fff_impl::handle_pdu, this, _1));
      message_port_register_out(PMTCONSTSTR__PDU_OUT);
    }

    /*
     * Our virtual destructor.
     */
    pdu_fir_filter_fff_impl::~pdu_fir_filter_fff_impl()
    {
    }

    void
    pdu_fir_filter_fff_impl::handle_pdu(pmt::pmt_t pdu)
    {
      // make sure PDU data is formed properly
      if (!(pmt::is_pair(pdu))) {
        GR_LOG_NOTICE(d_logger, "received unexpected PMT (non-pair)");
        return;
      }

      pmt::pmt_t meta = pmt::car(pdu);
      pmt::pmt_t v_data = pmt::cdr(pdu);

      if (!(is_dict(meta) && pmt::is_f32vector(v_data))) {
        GR_LOG_WARN(d_logger, "PMT is not a float PDU, dropping");
        return;
      }

      uint32_t v_len = pmt::length(v_data);
      size_t v_itemsize = pmt::uniform_vector_itemsize(v_data);
      //std::cout << "got uniform vector of length " << v_len << " and itemsize " << v_itemsize << std::endl;

      const std::vector<float> d_in = pmt::f32vector_elements(v_data);
      std::vector<float> d_out, in_data; // output
      //const float z=0.0;
      int start = (d_taps.size() - 1)/2;

      // adjust length by filter taps?
      //d_out.resize(v_len + start*2);
      d_out.resize(v_len);
      in_data.clear();
      for (int ii=0; ii < start; ii++) in_data.insert(in_data.end(), d_in[0]);
      in_data.insert(in_data.end(), d_in.begin(), d_in.end());
      for (int ii=0; ii < start; ii++) in_data.insert(in_data.end(), d_in.back());

      // do FIR filtering
      //d_fir->filterN(&d_out[0], &in_data[0], v_len);
      d_fir->filterN(d_out.data(), in_data.data(), v_len);  // C++11??
      GR_LOG_DEBUG(d_logger, boost::format("Filterint for %d output samples, (%d in, %d out)") % v_len % in_data.size() % d_out.size());

      message_port_pub(PMTCONSTSTR__PDU_IN, (pmt::cons(meta, pmt::init_f32vector(v_len, (const float *)&d_out[0]) )));
    }

  } /* namespace pdu_utils */
} /* namespace gr */
