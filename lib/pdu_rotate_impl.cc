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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "pdu_rotate_impl.h"
#include "pdu_utils/constants.h"

namespace gr {
  namespace pdu_utils {

    pdu_rotate::sptr
    pdu_rotate::make(double phase_inc)
    {
      return gnuradio::get_initial_sptr
        (new pdu_rotate_impl(phase_inc));
    }


    /*
     * The private constructor
     */
    pdu_rotate_impl::pdu_rotate_impl(double phase_inc)
      : gr::sync_block("pdu_rotate",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0))
    {
    set_phase_inc(phase_inc);

    // register message ports
    message_port_register_in(PMTCONSTSTR__pdu_in());
    set_msg_handler(
        PMTCONSTSTR__pdu_in(),
        boost::bind(&pdu_rotate_impl::msg_handler, this, _1));
    message_port_register_out(PMTCONSTSTR__pdu_out());
    }

    /*
     * Our virtual destructor.
     */
    pdu_rotate_impl::~pdu_rotate_impl()
    {
    }

    void pdu_rotate_impl::set_phase_inc(double phase_inc)
    {
        d_r.set_phase_incr(exp(gr_complex(0, phase_inc)));
        d_phase_inc = phase_inc;
    }

    void
    pdu_rotate_impl::msg_handler(pmt::pmt_t pdu)
    {
      // make sure PDU data is formed properly
      if (!(pmt::is_pair(pdu))) {
          GR_LOG_NOTICE(d_logger, "received unexpected PMT (non-pair)");
          return;
      }

      // extract
      pmt::pmt_t meta = pmt::car(pdu);
      pmt::pmt_t v_data = pmt::cdr(pdu);

      // check PMT vector type
      if (!pmt::is_c32vector(v_data)) {
          GR_LOG_WARN(d_logger, "Failed to rotate the PDU because it is not a c32 vector");
          return;
      }

      // if "phase_inc" specified in metadata, use that field to set phase_inc temporarily
      bool use_tmp_phase_inc = false;
      if (pmt::dict_has_key(meta, PMTCONSTSTR__phase_inc())) {
        double tmp_phase_inc = pmt::to_double(pmt::dict_ref(meta, PMTCONSTSTR__phase_inc(), pmt::PMT_NIL));
        d_r.set_phase_incr(exp(gr_complex(0, tmp_phase_inc)));
        use_tmp_phase_inc = true;
      }

      // Extract data
      //std::vector<gr_complex> in_data = pmt::c32vector_elements(v_data);
      size_t len = 0;
      const gr_complex* in_data = pmt::c32vector_elements(v_data, len);
      std::vector<gr_complex> out_data(len);

      // reset phase and rotate
      d_r.set_phase(gr_complex(1, 0));
      d_r.rotateN(out_data.data(), in_data, len);

      // reset the phase increment if it was temporarily set
      if (use_tmp_phase_inc) {
        d_r.set_phase_incr(exp(gr_complex(0, d_phase_inc)));
      }

      // publish the new pdu
      message_port_pub( PMTCONSTSTR__pdu_out(),(pmt::cons(meta, pmt::init_c32vector(out_data.size(), out_data))));
    }

    int
    pdu_rotate_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      return noutput_items;
    }

  } /* namespace pdu_utils */
} /* namespace gr */

