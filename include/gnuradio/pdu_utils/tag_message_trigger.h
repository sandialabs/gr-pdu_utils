/* -*- c++ -*- */
/*
 * Copyright 2018-2021 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_PDU_UTILS_TAG_MESSAGE_TRIGGER_H
#define INCLUDED_PDU_UTILS_TAG_MESSAGE_TRIGGER_H

#include <gnuradio/sync_block.h>
#include <gnuradio/pdu_utils/api.h>

namespace gr {
namespace pdu_utils {

/*!
 * \brief Tag initiated emitter
 * \ingroup pdu_utils
 *
 * This block emits a predefined message when a tag is received
 * with a matching key on the streaming input port. Will not trigger
 * twice within holdoff samples, and will disarm after holdoff
 * samples unless holdoff is set to '0' in which case it will be
 * ignored.
 *
 */
template <class T>
class PDU_UTILS_API tag_message_trigger : virtual public gr::sync_block
{
public:
    typedef std::shared_ptr<tag_message_trigger<T>> sptr;

    /**
     * Creates a new instance of tag_message_trigger
     *
     * @param trigger_key -
     * @param arming_key -
     * @param message -
     * @param holdoff -
     * @param samp_rate -
     * @param start_time -
     * @param delay_time -
     * @param tpdu_mode -
     */
    static sptr make(pmt::pmt_t trigger_key,
                     pmt::pmt_t arming_key,
                     pmt::pmt_t message,
                     uint32_t holdoff,
                     double samp_rate = 0,
                     double start_time = 0,
                     double delay_time = 0,
                     bool tpdu_mode = false);

    virtual void set_trigger_key(pmt::pmt_t) = 0;
    virtual void set_arming_key(pmt::pmt_t) = 0;
    virtual void set_time_tag_key(pmt::pmt_t) = 0;
    virtual void set_message(pmt::pmt_t) = 0;
    virtual void set_holdoff(uint32_t) = 0;
    virtual void set_samp_rate(double) = 0;
    virtual void set_start_time(double) = 0;
    virtual void set_delay_time(double) = 0;
    virtual void set_tx_limit(uint64_t) = 0;
};

typedef tag_message_trigger<unsigned char> tag_message_trigger_b;
typedef tag_message_trigger<short> tag_message_trigger_s;
typedef tag_message_trigger<int32_t> tag_message_trigger_i;
typedef tag_message_trigger<float> tag_message_trigger_f;
typedef tag_message_trigger<gr_complex> tag_message_trigger_c;
} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_TAG_MESSAGE_TRIGGER_H */
