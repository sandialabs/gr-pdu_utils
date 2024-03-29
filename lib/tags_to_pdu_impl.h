/* -*- c++ -*- */
/*
 * Copyright 2018-2021 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_PDU_UTILS_TAGS_TO_PDU_IMPL_H
#define INCLUDED_PDU_UTILS_TAGS_TO_PDU_IMPL_H

#include <gnuradio/pdu_utils/constants.h>
#include <gnuradio/pdu_utils/tags_to_pdu.h>

namespace gr {
namespace pdu_utils {

template <class T>
class tags_to_pdu_impl : public tags_to_pdu<T>
{
private:
    // overloaded pmt uniform vector initializers
    inline pmt::pmt_t init_data(std::vector<unsigned char> data)
    {
        return pmt::init_u8vector(data.size(), (const unsigned char*)&data[0]);
    }
    inline pmt::pmt_t init_data(std::vector<short> data)
    {
        return pmt::init_s16vector(data.size(), (const short*)&data[0]);
    }
    inline pmt::pmt_t init_data(std::vector<float> data)
    {
        return pmt::init_f32vector(data.size(), (const float*)&data[0]);
    }
    inline pmt::pmt_t init_data(std::vector<gr_complex> data)
    {
        return pmt::init_c32vector(data.size(), (const gr_complex*)&data[0]);
    }

private:
    pmt::pmt_t d_sob_tag_key;
    pmt::pmt_t d_eob_tag_key;
    pmt::pmt_t d_time_tag_key;
    uint32_t d_max_pdu_size;
    double d_samp_rate;
    std::vector<T> d_prepend;
    bool d_pub_sobs;
    uint32_t d_tail_size;

    bool d_triggered;
    uint64_t d_burst_counter;


    uint64_t d_known_time_int_sec; // known integer seconds of a particular item
    double d_known_time_frac_sec;  // known fractional seconds of a particular item
    uint64_t d_known_time_offset;  // known item offset of a particular item

    uint32_t d_eob_alignment;
    uint32_t d_eob_offset;
    uint64_t d_sob_tag_offset;

    std::vector<T> d_vector;
    pmt::pmt_t d_meta_dict;
    std::vector<tag_t> d_tags;
    tag_t d_tag;

    bool d_wall_clock_time;
    boost::posix_time::ptime d_epoch;

    enum TAG_TYPE { NONE = 0, SOB, EOB } d_tag_type;

    void publish_message(void);
    void set_known_time_offset(uint64_t, double, uint64_t);

    void handle_ctrl_msg(pmt::pmt_t ctrl_msg);

public:
    tags_to_pdu_impl(pmt::pmt_t start_tag,
                     pmt::pmt_t end_tag,
                     uint32_t max_pdu_size,
                     double samp_rate,
                     std::vector<T> prepend,
                     bool pub_sobs,
                     uint32_t tail_size,
                     double start_time);

    ~tags_to_pdu_impl() override;

    // public callbacks
    void set_start_tag(pmt::pmt_t) override;
    void set_end_tag(pmt::pmt_t) override;
    void set_time_tag_key(pmt::pmt_t) override;
    void set_samp_rate(double) override;
    void set_start_time(double) override;
    void set_prepend(std::vector<T>) override;
    void set_tail_size(uint32_t) override;
    void set_max_pdu_size(uint32_t) override;
    void publish_sob_msgs(bool pub) override { d_pub_sobs = pub; };
    void set_eob_parameters(uint32_t, uint32_t) override;
    void enable_time_debug(bool) override;
    uint32_t get_eob_offset() override { return d_eob_offset; };
    uint32_t get_eob_alignment() override { return d_eob_alignment; };

    // Where all the action really happens
    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items) override;
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_TAGS_TO_PDU_IMPL_H */
