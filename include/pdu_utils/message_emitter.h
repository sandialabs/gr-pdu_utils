/* -*- c++ -*- */
/*
 * Copyright 2018, 2019, 2020 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */


#ifndef INCLUDED_PDU_UTILS_MESSAGE_EMITTER_H
#define INCLUDED_PDU_UTILS_MESSAGE_EMITTER_H

#include <gnuradio/block.h>
#include <pdu_utils/api.h>

namespace gr {
namespace pdu_utils {

/*!
 * \brief Block emits messages when emit() is called. Designed to be useful
 * for QA tests.
 *
 * \ingroup pdu_utils
 *
 */
class PDU_UTILS_API message_emitter : virtual public gr::block
{
public:
    typedef boost::shared_ptr<message_emitter> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of pdu_utils::message_emitter.
     *
     * @param msg - PDU message to emit
     */
    static sptr make(pmt::pmt_t msg = pmt::PMT_NIL);

    /**
     * Set Message to emit
     *
     * @param msg - message to emit
     */
    virtual void set_msg(pmt::pmt_t msg) = 0;

    /**
     * Returns count of total messages emitted
     *
     * @return uint64_t
     */
    virtual uint64_t get_n_msgs(void) = 0;

    /**
     * Emits pre-specified message
     */
    virtual void emit(void) = 0;

    /**
     * Emits passed message.
     * If passed message is invalid, pre-specified message is emitted
     *
     * @param msg - Message to emit
     */
    virtual void emit(pmt::pmt_t msg) = 0;
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_MESSAGE_EMITTER_H */
