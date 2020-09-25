/* -*- c++ -*- */
/*
 * Copyright 2018, 2019, 2020 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_PDU_UTILS_MSG_DROP_RANDOM_H
#define INCLUDED_PDU_UTILS_MSG_DROP_RANDOM_H

#include <gnuradio/block.h>
#include <pdu_utils/api.h>

namespace gr {
namespace pdu_utils {

/*!
 * \brief Will randomly drop a message with specified probability.
 * \ingroup pdu_utils
 *
 */
class PDU_UTILS_API msg_drop_random : virtual public gr::block
{
public:
    typedef boost::shared_ptr<msg_drop_random> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of pdu_utils::msg_drop_random.
     *
     * @param p_drop - percentage to drop, [0..1]
     * @param seed - RNG seed value
     */
    static sptr make(float p_drop, uint64_t seed = 12345678);

    /**
     * Set percentage of dropped messages
     *
     * @param p - percentage drop, [0..1]
     */
    virtual void set_prob_drop(float p) = 0;

    /**
     * Resets counters
     */
    virtual void reset(void) = 0;

    /**
     * Returns count of dropped messages
     *
     * @return uint64_t
     */
    virtual uint64_t get_drop_count() = 0;

    /**
     * Returns count of incoming messages
     *
     * @return uint64_t
     */
    virtual uint64_t get_msg_count() = 0;

    /**
     * Returns count of passed messages
     *
     * @return uint64_t
     */
    virtual uint64_t get_pass_count() = 0;
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_MSG_DROP_RANDOM_H */
