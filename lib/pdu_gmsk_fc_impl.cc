/* -*- c++ -*- */
/*
 * Copyright 2018, 2019, 2020 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "pdu_gmsk_fc_impl.h"
#include <gnuradio/io_signature.h>

#include <gnuradio/blocks/pdu.h>
#include <gnuradio/fxpt.h>
#include <cmath>


namespace gr {
namespace pdu_utils {

pdu_gmsk_fc::sptr pdu_gmsk_fc::make(float sensitivity, const std::vector<float> taps)
{
    return gnuradio::get_initial_sptr(new pdu_gmsk_fc_impl(sensitivity, taps));
}

/*
 * The private constructor
 */
pdu_gmsk_fc_impl::pdu_gmsk_fc_impl(float sensitivity, const std::vector<float> taps)
    : gr::block("pdu_gmsk_fc",
                gr::io_signature::make(0, 0, 0),
                gr::io_signature::make(0, 0, 0)),
      d_phase(0),
      d_sensitivity(sensitivity),
      d_taps(taps)
{
    d_fir = new filter::kernel::fir_filter_fff(1, taps);


    // table of a log-ramp for scaling bursts
    // [10**((x-50)/10.) for x in range(0,50)]
    d_log_ramp.clear();
    for (size_t i = 0; i < 50; i++) {
        d_log_ramp.push_back(std::pow(10, ((float)i - 50.0) / 10.0));
    }

    message_port_register_in(PMTCONSTSTR__PDU_IN);
    set_msg_handler(PMTCONSTSTR__PDU_IN,
                    boost::bind(&pdu_gmsk_fc_impl::handle_pdu, this, _1));
    message_port_register_out(PMTCONSTSTR__PDU_OUT);
}

/*
 * Our virtual destructor.
 */
pdu_gmsk_fc_impl::~pdu_gmsk_fc_impl() {}

void pdu_gmsk_fc_impl::handle_pdu(pmt::pmt_t pdu)
{
    // make sure PDU data is formed properly
    if (!(pmt::is_pair(pdu))) {
        GR_LOG_NOTICE(d_logger, "received unexpected PMT (non-pair)");
        return;
    }

    // gr::thread::scoped_lock l(d_setlock);

    /* code */
    pmt::pmt_t meta = pmt::car(pdu);
    pmt::pmt_t v_data = pmt::cdr(pdu);

    if (pmt::is_f32vector(v_data)) {
        uint32_t v_len = pmt::length(v_data);
        size_t offset = 0;

        const std::vector<float> d_in = pmt::f32vector_elements(v_data);

        std::vector<gr_complex> mod; // output
        int start = d_taps.size() - 1;
        // adjust length by filter taps
        v_len -= start;
        mod.resize(v_len);

        // do FIR filtering
        for (int ii = 0; ii < v_len; ii++) {
            if (d_taps.size() > 1) {
                float in_filt = 0;

                /* THE OLD WAY, changed to use gr::filter
                for (int jj = 0; jj < d_taps.size(); jj++) {
                  in_filt += d_in[ii + start - jj] * d_taps[jj];
                } */

                in_filt = d_fir->filter(&d_in[ii]);

                d_phase = d_phase + d_sensitivity * in_filt;
            } else {
                d_phase = d_phase + d_sensitivity * d_in[ii];
            }

// place phase in [-pi, +pi)
#define F_PI ((float)(M_PI))

            float oi, oq;
            float p_mod = std::fmod(d_phase + F_PI, 2.0f * F_PI) - F_PI;

            // int32_t angle = gr::fxpt::float_to_fixed (d_phase);
            int32_t angle = gr::fxpt::float_to_fixed(p_mod);
            gr::fxpt::sincos(angle, &oq, &oi);

            // mod[ii] = gr_complex(oi, oq);
            mod[ii] = std::complex<float>(oi, oq);
        }

        // make vectors same size (by dumb end-extension)
        gr_complex fill_val = mod.back();
        for (int ii = 0; ii < start; ii++) {
            mod.push_back(fill_val);
            v_len++;
        }

        // scale the first HARDCODE 50 samples with a simple ramp function
        size_t end_samp = std::min(d_log_ramp.size(), mod.size());
        for (size_t i = 0; i < end_samp; i++) {
            mod[i] *= d_log_ramp[i];
        }

        message_port_pub(
            PMTCONSTSTR__PDU_OUT,
            (pmt::cons(meta, pmt::init_c32vector(v_len, (const gr_complex*)&mod[0]))));
        d_phase = 0;

    } else {
        GR_LOG_WARN(d_logger, "Got unknown PDU vector type, dropping");
    }
}


void pdu_gmsk_fc_impl::set_sensitivity(float sensitivity)
{
    // gr::thread::scoped_lock l(d_setlock);

    d_sensitivity = sensitivity;
}


void pdu_gmsk_fc_impl::set_taps(std::vector<float> taps)
{
    // gr::thread::scoped_lock l(d_setlock);

    d_taps = taps;
}

} /* namespace pdu_utils */
} /* namespace gr */
