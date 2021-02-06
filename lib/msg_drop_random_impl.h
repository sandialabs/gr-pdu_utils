/* -*- c++ -*- */
/*
 * Copyright 2018-2021 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
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

    /*!
     * \brief Message handler for input messages
     *
     * \param msg Any PMT message passed from the scheduler's message handling.
     */
    void handle_msg(pmt::pmt_t msg);

public:
    msg_drop_random_impl(float p_drop, uint64_t seed);

    ~msg_drop_random_impl() override;
    bool stop() override; // overloaded for print output

    void set_prob_drop(float p) override;
    void reset(void) override;
    uint64_t get_drop_count() override { return d_drop_ctr; }
    uint64_t get_msg_count() override { return d_msg_ctr; }
    uint64_t get_pass_count() override { return d_pass_ctr; }
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_MSG_DROP_RANDOM_IMPL_H */
