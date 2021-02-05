/* -*- c++ -*- */
/*
 * Copyright 2018-2021 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_PDU_UTILS_PDU_ADD_NOISE_H
#define INCLUDED_PDU_UTILS_PDU_ADD_NOISE_H

#include <gnuradio/block.h>
#include <pdu_utils/api.h>

namespace gr {
namespace pdu_utils {

/*!
 * \brief Add noise to PDU messages
 * \ingroup pdu_utils
 *
 * Adds randomly generated noise to u8, f32 or c32 PDUs, can also add an
 * offset or scale factor. Noise is applied first, then scaling, then offset.
 *
 * Noise Level is multiplied against the generated normal/uniform distribution sample. In
 * this way, it represents the standard deviation, or sqrt(variance).
 *
 * Distribution selection is limited to uniform or gaussian.
 *
 * Complex noise is generated as a random magnitude and phase.
 *
 * U8 noise is binary.
 *
 */
class PDU_UTILS_API pdu_add_noise : virtual public gr::block
{
public:
    typedef std::shared_ptr<pdu_add_noise> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of pdu_utils::pdu_add_noise.
     *
     *
     * @param noise_level - noise scaling (std dev)
     * @param offset - signal shifting
     * @param scale - signal scaling
     * @param seed - RNG seed
     * @param dist - noise distribution from enum #noise_dist
     */
    static sptr make(
        float noise_level, float offset, float scale, long seed = 12345678, int dist = 0);

    /**
     * Set Noise Level
     *
     * @param nl - noise level
     */
    virtual void set_noise_level(float nl) = 0;

    /**
     * Set offset, signal shifting
     *
     * @param o - offset
     */
    virtual void set_offset(float o) = 0;

    /**
     * Set Scale factor
     *
     * @param s - scale
     */
    virtual void set_scale(float s) = 0;

    /**
     * Set noise distribution
     *
     * @param d - #noise_dist
     */
    virtual void set_noise_dist(int d) = 0;

    /**
     * Set RNG seed
     *
     * @param x - seed
     */
    virtual void set_seed(int x) = 0;
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_ADD_NOISE_H */
