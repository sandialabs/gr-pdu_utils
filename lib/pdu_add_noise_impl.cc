/* -*- c++ -*- */
/*
 * Copyright 2018-2021 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "pdu_add_noise_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace pdu_utils {

pdu_add_noise::sptr
pdu_add_noise::make(float noise_level, float offset, float scale, long seed, int dist)
{
    return gnuradio::make_block_sptr<pdu_add_noise_impl>(
        noise_level, offset, scale, seed, dist);
}

/*
 * The private constructor
 */
pdu_add_noise_impl::pdu_add_noise_impl(
    float noise_level, float offset, float scale, long seed, int dist)
    : gr::block(
          "pdu_add_noise", io_signature::make(0, 0, 0), io_signature::make(0, 0, 0)),
      d_offset(offset),
      d_scale(scale),
      d_rng(seed)
{
    set_noise_dist(dist);
    set_noise_level(noise_level);
    message_port_register_in(PMTCONSTSTR__pdu_in());
    set_msg_handler(PMTCONSTSTR__pdu_in(),
                    [this](pmt::pmt_t msg) { this->handle_msg(msg); });
    message_port_register_out(PMTCONSTSTR__pdu_out());
}

/*
 * Our virtual destructor.
 */
pdu_add_noise_impl::~pdu_add_noise_impl() {}


void pdu_add_noise_impl::handle_msg(pmt::pmt_t pdu)
{
    // make sure PDU data is formed properly
    if (!(pmt::is_pair(pdu))) {
        GR_LOG_NOTICE(d_logger, "received unexpected PMT (non-pair)");
        return;
    }

    gr::thread::scoped_lock l(d_setlock);

    /* code */
    pmt::pmt_t meta = pmt::car(pdu);
    pmt::pmt_t v_data = pmt::cdr(pdu);

    if (pmt::is_u8vector(v_data)) {
        const std::vector<uint8_t> input = pmt::u8vector_elements(v_data);
        uint32_t v_len = input.size();
        std::vector<uint8_t> out;
        out.resize(v_len);

        for (int ii = 0; ii < v_len; ii++) {
            // u8 noise is [0-1]
            out[ii] = uint8_t(
                (input[ii] + (((get_rand_samp() + 1) / 2) * d_noise_level)) * d_scale +
                d_offset);
        }
        message_port_pub(PMTCONSTSTR__pdu_out(),
                         (pmt::cons(meta, pmt::init_u8vector(v_len, out))));

    } else if (pmt::is_f32vector(v_data)) {
        const std::vector<float> input = pmt::f32vector_elements(v_data);
        uint32_t v_len = input.size();
        std::vector<float> out;
        out.resize(v_len);

        for (int ii = 0; ii < v_len; ii++) {
            out[ii] =
                (input[ii] + (get_rand_samp() * d_noise_level)) * d_scale + d_offset;
        }
        message_port_pub(PMTCONSTSTR__pdu_out(),
                         (pmt::cons(meta, pmt::init_f32vector(v_len, out))));

    } else if (pmt::is_c32vector(v_data)) {
        const std::vector<gr_complex> input = pmt::c32vector_elements(v_data);
        uint32_t v_len = input.size();
        std::vector<gr_complex> out;
        out.resize(v_len);

        for (int ii = 0; ii < v_len; ii++) {
            out[ii].real((input[ii].real() + (get_rand_samp() * d_complex_nl)) * d_scale +
                         d_offset);
            out[ii].imag((input[ii].imag() + (get_rand_samp() * d_complex_nl)) * d_scale +
                         d_offset);
        }

        message_port_pub(PMTCONSTSTR__pdu_out(),
                         (pmt::cons(meta, pmt::init_c32vector(v_len, out))));

    } else {
        GR_LOG_WARN(d_logger, "unsupported PDU type received");
    }
}

float pdu_add_noise_impl::get_rand_samp()
{
    float r_samp = 0;
    if (d_noise_dist == UNIFORM) {
        r_samp = d_rng.ran1() * 2 - 1;
    } else if (d_noise_dist == GAUSSIAN) {
        r_samp = d_rng.gasdev();
    }

    return r_samp;
}

void pdu_add_noise_impl::set_noise_level(float nl)
{
    gr::thread::scoped_lock l(d_setlock);

    d_noise_level = nl;
    d_complex_nl = (float)(nl / std::sqrt(2.0));
}

void pdu_add_noise_impl::set_offset(float o)
{
    gr::thread::scoped_lock l(d_setlock);

    d_offset = o;
}

void pdu_add_noise_impl::set_scale(float s)
{
    gr::thread::scoped_lock l(d_setlock);

    d_scale = s;
}

void pdu_add_noise_impl::set_noise_dist(int d)
{
    gr::thread::scoped_lock l(d_setlock);
    d_noise_dist = static_cast<noise_dist>(d);
}

void pdu_add_noise_impl::set_seed(int x)
{
    gr::thread::scoped_lock l(d_setlock);
    d_rng.reseed(x);
}

} /* namespace pdu_utils */
} /* namespace gr */
