/* -*- c++ -*- */
/*
 * Copyright 2018, 2019, 2020 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_PDU_UTILS_MESSAGE_COUNTER_IMPL_H
#define INCLUDED_PDU_UTILS_MESSAGE_COUNTER_IMPL_H

#include <pdu_utils/constants.h>
#include <pdu_utils/message_counter.h>

namespace gr {
namespace pdu_utils {
/**
 * Implementing class of message_counter
 */
class message_counter_impl : public message_counter
{
private:
    uint64_t d_ctr;
    std::string d_name;

public:
    /**
     * Constructor
     *
     * @param name - name of counter
     */
    message_counter_impl(std::string name);

    /**
     * deconstructor
     */
    ~message_counter_impl();
    virtual bool stop(); // overloaded for print output
    void setup_rpc();    // enable controlport

    /**
     * message handler
     *
     * @param msg - incoming message
     */
    void handle_msg(pmt::pmt_t msg);

    /**
     * resets the counter to zero
     */
    virtual void reset(void) { d_ctr = 0; }


    /**
     * returns the current value of the counter
     *
     * @return uint64_t - counter value.
     */
    virtual uint64_t get_ctr() { return d_ctr; }

    /**
     * returns the counter name
     *
     * @return pmt::pmt_t
     */
    virtual std::string get_name(void) { return d_name; }

}; // end class message_counter_impl

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_MESSAGE_COUNTER_IMPL_H */
