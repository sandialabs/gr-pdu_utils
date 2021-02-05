/* -*- c++ -*- */
/*
 * Copyright 2018-2021 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */


#ifndef INCLUDED_PDU_UTILS_MESSAGE_COUNTER_H
#define INCLUDED_PDU_UTILS_MESSAGE_COUNTER_H

#include <gnuradio/block.h>
#include <pdu_utils/api.h>
#include <string>


namespace gr {
namespace pdu_utils {

/*!
 * \brief Counts the number of message received by the msg port
 *
 * Prints out message count when the flow graph stops
 *
 * \ingroup pdu_utils
 *
 */
class PDU_UTILS_API message_counter : virtual public gr::block
{
public:
    typedef std::shared_ptr<message_counter> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of pdu_utils::message_counter.
     *
     * To avoid accidental use of raw pointers, pdu_utils::message_counter's
     * constructor is in a private implementation
     * class. pdu_utils::message_counter::make is the public interface for
     * creating new instances.
     *
     * @param name - name of counter
     */
    static sptr make(std::string name);

    /**
     * resets the counter to zero
     */
    virtual void reset(void) = 0;

    /**
     * returns the current value of the counter
     *
     * @return uint64_t - counter value.
     */
    virtual uint64_t get_ctr(void) = 0;

    /**
     * returns the counter name
     *
     * @return std::string
     */
    virtual std::string get_name(void) = 0;
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_MESSAGE_COUNTER_H */
