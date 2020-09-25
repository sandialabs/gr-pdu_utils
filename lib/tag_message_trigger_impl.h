/* -*- c++ -*- */
/*
 * Copyright 2018, 2019, 2020 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_PDU_UTILS_TAG_MESSAGE_TRIGGER_IMPL_H
#define INCLUDED_PDU_UTILS_TAG_MESSAGE_TRIGGER_IMPL_H

#include <pdu_utils/constants.h>
#include <pdu_utils/tag_message_trigger.h>

namespace gr {
namespace pdu_utils {

template <class T>
class tag_message_trigger_impl : public tag_message_trigger<T>
{
private:
    pmt::pmt_t d_message;      // message to send
    pmt::pmt_t d_trigger_key;  // tag key to trigger message
    pmt::pmt_t d_arming_key;   // tag key to arm message trigger
    pmt::pmt_t d_time_tag_key; // tag key to arm message trigger
    double d_samp_rate;
    double d_start_time;
    bool d_tpdu_mode;
    uint64_t d_disarm_offset; // offset the block should disarm at
    bool d_fire_at_will;      // if set, fire anytime trigger is received
    bool d_armed;             // armed state
    uint64_t d_tx_limit;      // prevent additional transmissions
    bool d_no_holdoff;
    uint32_t d_holdoff; // holdoff - maximum arming time if there is a
                        // a trigger, or if not minimum interval in
                        // samples between trigger events
    uint64_t d_last_trigger_offset;

    uint64_t d_known_time_int_sec; // known integer seconds of a particular item
    double d_known_time_frac_sec;  // known fractional seconds of a particular item
    uint64_t d_known_time_offset;  // known item offset of a particular item
    uint64_t d_delay_time_int_sec; // timed PDU mode delay
    double d_delay_time_frac_sec;  // timed PDU mode delay

    tag_t d_tag;
    std::vector<tag_t> d_tags;

    void set_known_time_offset(uint64_t, double, uint64_t);


public:
    /**
     * Constructor
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
    tag_message_trigger_impl(pmt::pmt_t trigger_key,
                             pmt::pmt_t arming_key,
                             pmt::pmt_t message,
                             uint32_t holdoff,
                             double samp_rate,
                             double start_time,
                             double delay_time,
                             bool tpdu_mode);

    /**
     * Deconstructor
     */
    ~tag_message_trigger_impl();

    // public callback setters
    void set_trigger_key(pmt::pmt_t key) { d_trigger_key = key; };
    void set_arming_key(pmt::pmt_t key) { d_arming_key = key; };
    void set_time_tag_key(pmt::pmt_t key) { d_time_tag_key = key; };
    void set_message(pmt::pmt_t);
    void set_holdoff(uint32_t);
    void set_samp_rate(double);
    void set_start_time(double);
    void set_delay_time(double);
    void set_tx_limit(uint64_t);

    // message port
    void control_input(pmt::pmt_t msg);

    // Where all the action really happens
    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_TAG_MESSAGE_TRIGGER_IMPL_H */
