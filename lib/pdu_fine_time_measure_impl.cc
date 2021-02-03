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

#include "pdu_fine_time_measure_impl.h"
#include <gnuradio/io_signature.h>
#include <volk/volk.h>

namespace gr {
namespace pdu_utils {

pdu_fine_time_measure::sptr pdu_fine_time_measure::make(float pre_burst_time,
                                                        float post_burst_time,
                                                        size_t average_width,
                                                        float buffer_percent)
{
    return gnuradio::get_initial_sptr(new pdu_fine_time_measure_impl(
        pre_burst_time, post_burst_time, average_width, buffer_percent));
}

/*
 * The private constructor
 */
pdu_fine_time_measure_impl::pdu_fine_time_measure_impl(float pre_burst_time,
                                                       float post_burst_time,
                                                       size_t average_width,
                                                       float buffer_percent)
    : gr::block("pdu_fine_time_measure",
                gr::io_signature::make(0, 0, 0),
                gr::io_signature::make(0, 0, 0)),
      d_pre_burst_time(pre_burst_time),
      d_post_burst_time(post_burst_time),
      d_average_size(average_width),
      d_buffer_percent(buffer_percent / 100.0)
{
    message_port_register_in(PMTCONSTSTR__pdu_in());
    message_port_register_out(PMTCONSTSTR__pdu_out());
    set_msg_handler(PMTCONSTSTR__pdu_in(),
                    boost::bind(&pdu_fine_time_measure_impl::pdu_handler, this, _1));
}

/*
 * Our virtual destructor.
 */
pdu_fine_time_measure_impl::~pdu_fine_time_measure_impl() {}

void pdu_fine_time_measure_impl::pdu_handler(pmt::pmt_t pdu)
{
    if (!pmt::is_pair(pdu)) {
        GR_LOG_WARN(d_logger, "PDU not a pair, dropping");
        return;
    }

    pmt::pmt_t metadata = pmt::car(pdu);
    pmt::pmt_t data = pmt::cdr(pdu);

    if (!pmt::is_dict(metadata)) {
        GR_LOG_WARN(d_logger, "PDU metadata not a dictionary, dropping");
        return;
    }

    if (!pmt::is_c32vector(data)) {
        GR_LOG_WARN(d_logger, "PDU data not complex, dropping");
        return;
    }
    /*pmt::pmt_t time_pmt = pmt::dict_ref(metadata, PMTCONSTSTR__start_time(), pmt::PMT_NIL);
    uint64_t seconds = pmt::to_uint64(pmt::tuple_ref(time_pmt, 0));
    double frac_seconds = pmt::to_double(pmt::tuple_ref(time_pmt, 1));*/

    double start_time =
        pmt::to_double(pmt::dict_ref(metadata, PMTCONSTSTR__start_time(), pmt::PMT_NIL));
    float sample_rate =
        pmt::to_float(pmt::dict_ref(metadata, PMTCONSTSTR__sample_rate(), pmt::PMT_NIL));
    float duration = pmt::to_float(pmt::dict_ref(metadata, PMTCONSTSTR__duration(), pmt::PMT_NIL));

    // Divide the burst into pre-noise/burst/post_noise
    // This is, in a way, an SNR calculation
    float noiseSum = 0, burstSum = 0;
    size_t burst_size;
    const gr_complex* burst =
        (const gr_complex*)pmt::c32vector_elements(data, burst_size);

    d_magnitude_squared_f.resize(burst_size);
    volk_32fc_magnitude_squared_32f(&d_magnitude_squared_f[0], burst, burst_size);

    // Convert pre/post burst time to samples and subtract off buffer percent
    size_t pre_burst_stop =
        (d_pre_burst_time * sample_rate) * (1. - d_buffer_percent / 2.0);
    size_t post_burst_start =
        (d_post_burst_time * sample_rate) * (1. - d_buffer_percent / 2.0);
    // The length of the burst without tails with buffer precent removed
    size_t burst_len =
        (burst_size - (d_pre_burst_time + d_post_burst_time) * sample_rate) *
        d_buffer_percent;
    // If pre/post burst time are different then the center of our burst will not be the
    // middle sample.
    size_t burst_center_offset = (d_pre_burst_time - d_post_burst_time) * sample_rate / 2;
    size_t burst_start = (burst_size - burst_len) / 2 + burst_center_offset;
    size_t burst_stop = (burst_size - burst_len) / 2 - burst_center_offset;

    for (size_t i = 0; i < pre_burst_stop; i++)
        noiseSum += d_magnitude_squared_f[i];
    for (size_t i = burst_size - post_burst_start; i < burst_size; i++)
        noiseSum += d_magnitude_squared_f[i];
    for (size_t i = burst_start; i < burst_size - burst_stop; i++)
        burstSum += d_magnitude_squared_f[i];

    if (pre_burst_stop + post_burst_start > 0)
        noiseSum /= float(pre_burst_stop + post_burst_start);
    if (burst_size - burst_start - burst_stop > 0)
        burstSum /= float(burst_size - burst_start - burst_stop);

    float thresh = (noiseSum + burstSum) / 2.0;
    float thresh2 = thresh * d_average_size;

    // Search for points above thresh
    size_t start = 0;
    // If noise_sum > burstSum, we aren't going to get a better estimate
    if (burstSum > noiseSum) {

        for (; start < burst_size; start++) {
            if (d_magnitude_squared_f[start] > thresh) {
                // We have a candidate.
                // Compute Moving average until we are above thresh
                float movAvg = 0;
                for (size_t j = 0; j < d_average_size; j++)
                    movAvg += d_magnitude_squared_f[start + j];
                // printf("start = %zu, movAvg = %f\n", start, movAvg);
                while (movAvg < thresh2 && start < burst_size - d_average_size) {
                    movAvg += d_magnitude_squared_f[start + d_average_size] -
                              d_magnitude_squared_f[start];
                    start++;
                }
                break;
            }
        }
    }
    // printf("start = %zu, %f, %f %zu\n", start, noiseSum, burstSum, pre_burst_stop);
    // if (start > 10) start -= 10;
    // else start = 0;
    // start = std::min(burst_size, start+150);
    /*frac_seconds += start / sample_rate;
    if (frac_seconds > 1) {
      seconds += int(frac_seconds);
      frac_seconds -= int(frac_seconds);
    }*/
    // start = 0;

    // Offset our burst time and remove samples from the beginning.
    metadata = pmt::dict_add(metadata,
                             PMTCONSTSTR__start_time(),
                             pmt::from_double(start_time + double(start) / sample_rate));
    metadata = pmt::dict_add(
        metadata, PMTCONSTSTR__duration(), pmt::from_float(duration - float(start) / sample_rate));
    // metadata = pmt::dict_add(metadata, PMTCONSTSTR__burst_time(),
    // pmt::make_tuple(pmt::from_uint64(seconds), pmt::from_double(frac_seconds)));
    pmt::pmt_t pdu_vector = pmt::init_c32vector(burst_size - start, burst + start);
    message_port_pub(PMTCONSTSTR__pdu_out(), pmt::cons(metadata, pdu_vector));
}


} /* namespace pdu_utils */
} /* namespace gr */
