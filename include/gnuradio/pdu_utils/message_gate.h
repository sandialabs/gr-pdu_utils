/* -*- c++ -*- */
/*
 * Copyright 2018-2021 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */


#ifndef INCLUDED_PDU_UTILS_MESSAGE_GATE_H
#define INCLUDED_PDU_UTILS_MESSAGE_GATE_H

#include <gnuradio/block.h>
#include <gnuradio/pdu_utils/api.h>

namespace gr {
namespace pdu_utils {

/*!
 * \brief Simple message gate
 * \ingroup pdu_utils
 *
 * Simple message gate to control whether or not messages pass.
 *
 */
class PDU_UTILS_API message_gate : virtual public gr::block
{
public:
    typedef std::shared_ptr<message_gate> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of pdu_utils::message_gate.
     *
     * @param enabled - true to pass messages
     */
    static sptr make(bool enabled);

    /**
     * Resets state of block
     */
    virtual void reset(void) = 0;

    /**
     * Return count of passed messages
     *
     * @return uint64_t
     */
    virtual uint64_t get_n_passed(void) = 0;

    /**
     * Return count of blocked messages
     *
     * @return uint64_t
     */
    virtual uint64_t get_n_blocked(void) = 0;

    /**
     * Returns enable status
     *
     * @return bool - true when messages passed
     */
    virtual bool get_enabled(void) = 0;

    /**
     * Set gate state
     *
     * @param enabled - true to pass messages
     */
    virtual void set_enabled(bool enabled) = 0;
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_MESSAGE_GATE_H */
