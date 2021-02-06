/* -*- c++ -*- */
/*
 * Copyright 2018-2021 National Technology & Engineering Solutions of Sandia, LLC
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

    /*!
     * \brief Message handler for input messages
     *
     * \param msg Any PMT message passed from the scheduler's message handling.
     */
    void handle_msg(pmt::pmt_t msg);

public:
    message_gate_impl(bool enabled);

    ~message_gate_impl() override;
    bool stop() override;      // overloaded for print output
    void setup_rpc() override; // enable controlport

    void reset(void) override;
    void set_enabled(bool enabled) override;
    bool get_enabled(void) override { return d_enabled; }
    uint64_t get_n_passed(void) override { return d_n_passed; }
    uint64_t get_n_blocked(void) override { return d_n_blocked; }
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_MESSAGE_GATE_IMPL_H */
