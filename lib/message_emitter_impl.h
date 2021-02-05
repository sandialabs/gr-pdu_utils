/* -*- c++ -*- */
/*
 * Copyright 2018-2021 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_PDU_UTILS_MESSAGE_EMITTER_IMPL_H
#define INCLUDED_PDU_UTILS_MESSAGE_EMITTER_IMPL_H

#include <pdu_utils/constants.h>
#include <pdu_utils/message_emitter.h>

namespace gr {
namespace pdu_utils {

class message_emitter_impl : public message_emitter
{
private:
    pmt::pmt_t d_msg;
    uint64_t d_n_msgs;

public:
    /*!
     * Constructor
     *
     * @param msg - PDU message to emit
     */
    message_emitter_impl(pmt::pmt_t msg = pmt::PMT_NIL);

    /**
     * Deconstructor
     */
    ~message_emitter_impl();
    bool stop();      // overloaded for print output
    void setup_rpc(); // enable controlport

    /**
     * Set Message to emit
     *
     * @param msg - message to emit
     */
    void set_msg(pmt::pmt_t msg);

    /**
     * Returns count of total messages emitted
     *
     * @return uint64_t
     */
    uint64_t get_n_msgs(void) { return d_n_msgs; }

    /**
     * Emits pre-specified message
     */
    void emit();

    /**
     * Emits passed message.
     * If passed message is invalid, pre-specified message is emitted
     *
     * @param msg - Message to emit
     */
    void emit(pmt::pmt_t msg);
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_MESSAGE_EMITTER_IMPL_H */
