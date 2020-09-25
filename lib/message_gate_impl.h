/* -*- c++ -*- */
/*
 * Copyright 2018, 2019, 2020 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_PDU_UTILS_MESSAGE_GATE_IMPL_H
#define INCLUDED_PDU_UTILS_MESSAGE_GATE_IMPL_H

#include <pdu_utils/constants.h>
#include <pdu_utils/message_gate.h>

namespace gr {
namespace pdu_utils {

class message_gate_impl : public message_gate
{
private:
    bool d_enabled;
    uint64_t d_n_passed;
    uint64_t d_n_blocked;

public:
    /**
     * Constructor
     *
     * @param enabled - true to pass messages
     */
    message_gate_impl(bool enabled);

    /**
     * Deconstructor
     */
    ~message_gate_impl();
    bool stop();      // overloaded for print output
    void setup_rpc(); // enable controlport

    void handle_msg(pmt::pmt_t msg);

    void reset(void);

    /**
     * Set gate state
     *
     * @param enabled - true to pass messages
     */
    void set_enabled(bool enabled);

    /**
     * Returns enable status
     *
     * @return bool - true when messages passed
     */
    bool get_enabled(void) { return d_enabled; }

    /**
     * Return count of passed messages
     *
     * @return uint64_t
     */
    uint64_t get_n_passed(void) { return d_n_passed; }

    /**
     * Return count of blocked messages
     *
     * @return uint64_t
     */
    uint64_t get_n_blocked(void) { return d_n_blocked; }
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_MESSAGE_GATE_IMPL_H */
