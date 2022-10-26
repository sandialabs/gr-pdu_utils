/* -*- c++ -*- */
/*
 * Copyright 2018-2021 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_PDU_UTILS_PDU_ADD_NOISE_IMPL_H
#define INCLUDED_PDU_UTILS_PDU_ADD_NOISE_IMPL_H

#include <gnuradio/random.h>
#include <gnuradio/pdu_utils/constants.h>
#include <gnuradio/pdu_utils/pdu_add_noise.h>

namespace gr {
namespace pdu_utils {

class pdu_add_noise_impl : public pdu_add_noise
{
private:
    float d_noise_level;
    float d_complex_nl;
    float d_offset;
    float d_scale;
    gr::random d_rng;
    noise_dist d_noise_dist;

    float get_rand_samp();
    void handle_msg(pmt::pmt_t pdu);

public:
    pdu_add_noise_impl(float noise_level, float offset, float scale, long seed, int dist);

    ~pdu_add_noise_impl();

    void set_noise_level(float nl) override;
    void set_offset(float o) override;
    void set_scale(float s) override;
    void set_noise_dist(int d) override;
    void set_seed(int x) override;
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_ADD_NOISE_IMPL_H */
