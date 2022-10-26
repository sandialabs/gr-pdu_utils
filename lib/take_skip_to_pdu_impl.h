/* -*- c++ -*- */
/*
 * Copyright 2018-2021 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_PDU_UTILS_TAKE_SKIP_TO_PDU_IMPL_H
#define INCLUDED_PDU_UTILS_TAKE_SKIP_TO_PDU_IMPL_H

#include <gnuradio/pdu_utils/constants.h>
#include <gnuradio/pdu_utils/take_skip_to_pdu.h>

namespace gr {
namespace pdu_utils {

template <class T>
class take_skip_to_pdu_impl : public take_skip_to_pdu<T>
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
    std::vector<T> d_vector;
    uint32_t d_take;
    uint32_t d_skip;
    uint64_t d_next;
    bool d_triggered;
    uint64_t d_burst_counter;
    uint8_t d_prev_byte;
    pmt::pmt_t d_meta_dict;

    void publish_message(void);

public:
    take_skip_to_pdu_impl(uint32_t take, uint32_t skip);

    ~take_skip_to_pdu_impl() override;

    void set_take(uint32_t take) override;
    void set_skip(uint32_t skip) override;

    // Where all the action really happens
    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items) override;
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_TAKE_SKIP_TO_PDU_IMPL_H */
