/* -*- c++ -*- */
/*
 * Copyright 2018, 2019, 2020 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_PDU_UTILS_PDU_HEAD_TAIL_IMPL_H
#define INCLUDED_PDU_UTILS_PDU_HEAD_TAIL_IMPL_H

#include <pdu_utils/constants.h>
#include <pdu_utils/pdu_head_tail.h>

namespace gr {
namespace pdu_utils {

class pdu_head_tail_impl : public pdu_head_tail
{
private:
    uint32_t d_input_type;
    uint32_t d_bit_order;
    uint32_t d_length;
    uint32_t d_histsize;
    uint32_t d_maxhistsize;
    bool d_space_bytes;

    void reset(void);

    std::vector<uint8_t> d_head;
    std::vector<uint8_t> d_tail;
    std::vector<float> d_head_f;
    std::vector<float> d_tail_f;

public:
    /**
     * Constructor
     *
     * @param input_type -
     * @param length -
     * @param histsize -
     */
    pdu_head_tail_impl(uint32_t input_type, uint32_t length, uint32_t histsize);

    /**
     * Deconstructor
     */
    ~pdu_head_tail_impl();

    void handle_pdu(pmt::pmt_t pdu);

    /**
     * Set Length
     *
     * @param length -
     */
    void set_length(uint32_t length);

    /**
     * Set Histogram size
     *
     * @param histsize -
     */
    void set_histsize(uint32_t histsize);
    void set_space_bytes(bool spacebytes);
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_HEAD_TAIL_IMPL_H */
