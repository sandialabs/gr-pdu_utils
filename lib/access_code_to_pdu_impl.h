/* -*- c++ -*- */
/*
 * Copyright 2018, 2019, 2020, 2022 National Technology & Engineering Solutions of Sandia,
 * LLC (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_PDU_UTILS_ACCESS_CODE_TO_PDU_IMPL_H
#define INCLUDED_PDU_UTILS_ACCESS_CODE_TO_PDU_IMPL_H

#include <gnuradio/pdu_utils/access_code_to_pdu.h>
#include <gnuradio/pdu_utils/constants.h>

namespace gr {
namespace pdu_utils {

class access_code_to_pdu_impl : public access_code_to_pdu
{
private:
    uint64_t d_data_reg;
    uint64_t d_burst_counter;
    bool d_reversed;
    uint64_t d_nread;
    uint64_t d_bit_index;
    uint32_t d_burst_len;
    uint32_t d_threshold;
    sync_mode d_syncmode;
    read_mode d_readmode;
    bool d_lock;

    uint64_t d_access_code;
    uint64_t d_access_mask;
    uint32_t d_access_len;

    uint64_t d_tail_sync;
    uint64_t d_tail_mask;
    uint32_t d_tail_len;

    std::vector<uint8_t> d_output;

    std::vector<uint32_t> d_burst_vector;
    std::vector<bool> d_reversed_vector;

public:
    access_code_to_pdu_impl(std::string access_code,
                            std::string tail_sync,
                            uint32_t burst_len,
                            uint32_t threshold,
                            sync_mode syncmode,
                            read_mode readmode);
    ~access_code_to_pdu_impl();

    void
    set_sync(std::string sync_string, uint64_t* syncword, uint64_t* mask, uint32_t* len);

    bool check_access_code();
    bool check_tail_sync();

    void publish_message();

    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_ACCESS_CODE_TO_PDU_IMPL_H */
