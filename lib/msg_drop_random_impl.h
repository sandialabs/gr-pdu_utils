/* -*- c++ -*- */
/*
 * Copyright 2018 National Technology & Engineering Solutions of Sandia, LLC (NTESS).
 * Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government retains
 * certain rights in this software.
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
#ifndef INCLUDED_PDU_UTILS_MSG_DROP_RANDOM_IMPL_H
#define INCLUDED_PDU_UTILS_MSG_DROP_RANDOM_IMPL_H

#include <gnuradio/random.h>
#include <pdu_utils/constants.h>
#include <pdu_utils/msg_drop_random.h>

namespace gr {
namespace pdu_utils {

class msg_drop_random_impl : public msg_drop_random
{
private:
    uint64_t d_msg_ctr;
    uint64_t d_pass_ctr;
    uint64_t d_drop_ctr;
    float d_p_drop;
    gr::random d_rng;

public:
    /**
     * Constructor
     *
     * @param p_drop - percentage to drop, [0..1]
     * @param seed - RNG seed value
     */
    msg_drop_random_impl(float p_drop, uint64_t seed);

    /**
     * Deconstructor
     */
    ~msg_drop_random_impl();
    bool stop(); // overloaded for print output

    void handle_msg(pmt::pmt_t msg);

    /**
     * Set percentage of dropped messages
     *
     * @param p - percentage drop, [0..1]
     */
    void set_prob_drop(float p);

    /**
     * Resets counters
     */
    void reset(void);

    /**
     * Returns count of dropped messages
     *
     * @return uint64_t
     */
    uint64_t get_drop_count() { return d_drop_ctr; }

    /**
     * Returns count of incoming messages
     *
     * @return uint64_t
     */
    uint64_t get_msg_count() { return d_msg_ctr; }

    /**
     * Returns count of passed messages
     *
     * @return uint64_t
     */
    uint64_t get_pass_count() { return d_pass_ctr; }
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_MSG_DROP_RANDOM_IMPL_H */
