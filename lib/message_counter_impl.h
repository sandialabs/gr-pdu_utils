/* -*- c++ -*- */
/*
 * Copyright 2018-2021 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_PDU_UTILS_MESSAGE_COUNTER_IMPL_H
#define INCLUDED_PDU_UTILS_MESSAGE_COUNTER_IMPL_H

#include <gnuradio/pdu_utils/constants.h>
#include <gnuradio/pdu_utils/message_counter.h>

namespace gr {
namespace pdu_utils {
/**
 * Implementing class of message_counter
 */
class message_counter_impl : public message_counter
{
private:
    std::string d_name;
    uint64_t d_ctr;

    /*!
     * \brief Message handler for input messages
     *
     * \param msg Any PMT message passed from the scheduler's message handling.
     */
    void handle_msg(pmt::pmt_t msg);

public:
    message_counter_impl(std::string name);

    ~message_counter_impl() override;
    virtual bool stop() override; // overloaded for print output
    void setup_rpc() override;    // enable controlport

    virtual void reset(void) override { d_ctr = 0; }
    virtual uint64_t get_ctr() override { return d_ctr; }
    virtual std::string get_name(void) override { return d_name; }

}; // end class message_counter_impl

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_MESSAGE_COUNTER_IMPL_H */
