/* -*- c++ -*- */
/*
 * <COPYRIGHT PLACEHOLDER>
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

#ifndef INCLUDED_PDU_UTILS_PDU_ADD_NOISE_IMPL_H
#define INCLUDED_PDU_UTILS_PDU_ADD_NOISE_IMPL_H

#include <gnuradio/random.h>
#include <pdu_utils/constants.h>
#include <pdu_utils/pdu_add_noise.h>

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

public:
    /**
     * Constructor
     *
     * @param noise_level - noise scaling (std dev)
     * @param offset - signal shifting
     * @param scale - signal scaling
     * @param seed - RNG seed
     * @param dist - noise distribution from enum #noise_dist
     */
    pdu_add_noise_impl(float noise_level, float offset, float scale, long seed, int dist);

    /**
     * Deconstructor
     */
    ~pdu_add_noise_impl();

    void handle_msg(pmt::pmt_t pdu);

    /**
     * Set Noise Level
     *
     * @param nl - noise level
     */
    void set_noise_level(float nl);

    /**
     * Set offset, signal shifting
     *
     * @param o - offset
     */
    void set_offset(float o);

    /**
     * Set Scale factor
     *
     * @param s - scale
     */
    void set_scale(float s);

    /**
     * Set noise distribution
     *
     * @param d - #noise_dist
     */
    void set_noise_dist(int d);

    /**
     * Set RNG seed
     *
     * @param x - seed
     */
    void set_seed(int x);
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_ADD_NOISE_IMPL_H */
