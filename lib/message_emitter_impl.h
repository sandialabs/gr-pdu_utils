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

#include <gnuradio/pdu_utils/constants.h>
#include <gnuradio/pdu_utils/message_emitter.h>

namespace gr {
namespace pdu_utils {

class message_emitter_impl : public message_emitter
{
private:
    pmt::pmt_t d_msg;
    uint64_t d_n_msgs;

public:
    message_emitter_impl(pmt::pmt_t msg = pmt::PMT_NIL);

    ~message_emitter_impl();
    bool stop() override;      // overloaded for print output
    void setup_rpc() override; // enable controlport

    void set_msg(pmt::pmt_t msg) override;
    uint64_t get_n_msgs(void) override { return d_n_msgs; }
    void emit() override;
    void emit(pmt::pmt_t msg) override;
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_MESSAGE_EMITTER_IMPL_H */
