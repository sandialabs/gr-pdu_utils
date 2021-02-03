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

#include "pdu_fir_filter_impl.h"
#include "volk/volk.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace pdu_utils {

pdu_fir_filter::sptr pdu_fir_filter::make(int decimation, const std::vector<float> taps)
{
    return gnuradio::get_initial_sptr(new pdu_fir_filter_impl(decimation, taps));
}

/*
 * The private constructor
 */
pdu_fir_filter_impl::pdu_fir_filter_impl(int decimation, const std::vector<float> taps)
    : gr::block("pdu_fir_filter",
                gr::io_signature::make(0, 0, 0),
                gr::io_signature::make(0, 0, 0)),
      d_fir_fff(1, taps),
      d_fir_ccf(1, taps),
      d_decimation(decimation)
{
    set_taps(taps);

    // to avoid memory access issues within the filterNdec function of gnuradio
    // pad the input at the front and back based on the volk alignemt
    d_pad = volk_get_alignment() / sizeof(float);

    message_port_register_in(PMTCONSTSTR__pdu_in());
    message_port_register_out(PMTCONSTSTR__pdu_out());
    set_msg_handler(PMTCONSTSTR__pdu_in(),
                    boost::bind(&pdu_fir_filter_impl::handle_pdu, this, _1));
}

/*
 * Our virtual destructor.
 */
pdu_fir_filter_impl::~pdu_fir_filter_impl() {}

void pdu_fir_filter_impl::handle_pdu(pmt::pmt_t pdu)
{
    gr::thread::scoped_lock guard(d_mutex);

    // make sure PDU data is formed properly
    if (!(pmt::is_pair(pdu))) {
        GR_LOG_NOTICE(d_logger, "received unexpected PMT (non-pair)");
        return;
    }

    pmt::pmt_t metadata = pmt::car(pdu);
    pmt::pmt_t pdu_data = pmt::cdr(pdu);

    if (!pmt::is_dict(metadata)) {
        GR_LOG_WARN(d_logger, "PDU metadata is not a dict, dropping");
        return;
    }

    /*
     * Update Metadata Dictionary As Required:
     *
     * If decimation is 1 and our taps are odd, we do not need to update any dictionary
     * keys - check this first to avoid unnecessary dictionary references as this
     * operation can be expensive for large dictionaries.
     */
    if ((d_decimation != 1) || d_even_num_taps) {
        // we need a sample_rate key to update either field
        if (pmt::dict_has_key(metadata, PMTCONSTSTR__sample_rate())) {
            double sample_rate = pmt::to_double(
                pmt::dict_ref(metadata, PMTCONSTSTR__sample_rate(), pmt::PMT_NIL));
            if ((d_decimation != 1)) {
                sample_rate /= d_decimation;
                metadata = pmt::dict_add(
                    metadata, PMTCONSTSTR__sample_rate(), pmt::from_double(sample_rate));
            }
            // if we need to adjust the start time due to an even number of taps, do so
            if (d_even_num_taps && pmt::dict_has_key(metadata, PMTCONSTSTR__start_time())) {
                double start_time = pmt::to_double(
                    pmt::dict_ref(metadata, PMTCONSTSTR__start_time(), pmt::PMT_NIL));
                start_time -= 0.5 / sample_rate;
                metadata = pmt::dict_add(
                    metadata, PMTCONSTSTR__start_time(), pmt::from_double(start_time));
            }
        }
    }

    /*
     * Perform FIR Filtering
     *
     * This is done with compensation for group delay; for odd numbers of taps the output
     * remains aligned with the input, and for even numbers of taps a half sample is added
     * to the output before and after the input data resulting in the length increasing by
     * one, and the first output sample being a half sample early relative to the first
     * sample in the input.
     *
     * Padding (d_pad) is included as a workaround for a bug in upstream FIR filter
     * kernels (see: https://github.com/gnuradio/gnuradio/issues/3393) and does not affect
     * timing or alignment.
     */
    size_t vlen_in;
    if (pmt::is_f32vector(pdu_data)) {
        const float* d_in_p = pmt::f32vector_elements(pdu_data, vlen_in);
        if (vlen_in <= d_fir_fff.ntaps()) {
            // not enough data to process
            return;
        }
        size_t vlen_out(d_even_num_taps ? vlen_in + 1 : vlen_in);

        std::vector<float> d_in(vlen_in + 2 * d_pad + 2 * d_group_delay_offset, 0);
        d_in.insert(
            d_in.begin() + d_pad + d_group_delay_offset, d_in_p, d_in_p + vlen_in);
        std::vector<float> d_out(vlen_out / d_decimation);

        // do FIR filtering
        d_fir_fff.filterNdec(
            d_out.data(), d_in.data() + d_pad, d_out.size(), d_decimation);

        message_port_pub(
            PMTCONSTSTR__pdu_out(),
            (pmt::cons(metadata, pmt::init_f32vector(d_out.size(), d_out.data()))));

    } else if (pmt::is_c32vector(pdu_data)) {
        const gr_complex* d_in_p = pmt::c32vector_elements(pdu_data, vlen_in);
        if (vlen_in <= d_fir_ccf.ntaps()) {
            // not enough data to process
            return;
        }
        size_t vlen_out(d_even_num_taps ? vlen_in + 1 : vlen_in);

        std::vector<gr_complex> d_in(vlen_in + 2 * d_pad + 2 * d_group_delay_offset, 0);
        d_in.insert(
            d_in.begin() + d_pad + d_group_delay_offset, d_in_p, d_in_p + vlen_in);
        std::vector<gr_complex> d_out(vlen_out / d_decimation);

        // do FIR filtering
        d_fir_ccf.filterNdec(
            d_out.data(), d_in.data() + d_pad, d_out.size(), d_decimation);

        message_port_pub(
            PMTCONSTSTR__pdu_out(),
            (pmt::cons(metadata, pmt::init_c32vector(d_out.size(), d_out.data()))));

    } else if (pmt::is_u8vector(pdu_data)) {
        const uint8_t* d_in_p = pmt::u8vector_elements(pdu_data, vlen_in);
        if (vlen_in <= d_fir_fff.ntaps()) {
            // not enough data to process
            return;
        }
        size_t vlen_out(d_even_num_taps ? vlen_in + 1 : vlen_in);

        std::vector<float> d_in(vlen_in + 2 * d_pad + 2 * d_group_delay_offset, 0);
        d_in.insert(
            d_in.begin() + d_pad + d_group_delay_offset, d_in_p, d_in_p + vlen_in);
        std::vector<float> d_out(vlen_out / d_decimation);

        // do FIR filtering
        d_fir_fff.filterNdec(
            d_out.data(), d_in.data() + d_pad, d_out.size(), d_decimation);

        std::vector<uint8_t> d_out_byte(d_out.begin(), d_out.end());
        message_port_pub(
            PMTCONSTSTR__pdu_out(),
            (pmt::cons(metadata,
                       pmt::init_u8vector(d_out_byte.size(), d_out_byte.data()))));
    } else {
        GR_LOG_WARN(d_logger, "PMT is not a byte, float, or complex PDU, dropping");
        return;
    }
}

void pdu_fir_filter_impl::set_taps(std::vector<float> taps)
{
    gr::thread::scoped_lock guard(d_mutex);

    size_t tap_len = taps.size();
    d_fir_fff.set_taps(taps);
    d_fir_ccf.set_taps(taps);

    if (tap_len % 2) {
        d_group_delay_offset = (tap_len - 1) / 2;
        d_even_num_taps = false;
    } else {
        d_group_delay_offset = (tap_len) / 2;
        d_even_num_taps = true;
        GR_LOG_WARN(d_logger,
                    "PERFORMANCE IMPACT: Even number of taps requires inefficient manual "
                    "adjustiment of burst time");
    }
}

} /* namespace pdu_utils */
} // namespace gr
