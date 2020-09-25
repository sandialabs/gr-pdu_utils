/* -*- c++ -*- */
/*
 * Copyright 2018, 2019, 2020 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_PDU_UTILS_PDU_ALIGN_IMPL_H
#define INCLUDED_PDU_UTILS_PDU_ALIGN_IMPL_H

#include <pdu_utils/pdu_align.h>

namespace gr {
namespace pdu_utils {

class pdu_align_impl : public pdu_align
{
private:
    void pdu_handler(pmt::pmt_t pdu);
    void update_time_metadata(pmt::pmt_t& metadata, int start_idx);

    std::vector<uint64_t> d_syncwords;
    std::vector<size_t> d_syncword_lens;
    std::vector<uint64_t> d_masks;

    int d_offset;
    int d_threshold;
    align_modes d_mode;

public:
    /**
     * Constructor
     *
     * @param syncwords - binary syncword to search for
     * @param threshold - number of bit errors allowed
     * @param offset - bit offset to pass through
     * @param mode - Align mode
     */
    pdu_align_impl(std::string syncwords, int threshold, int offset, align_modes mode);

    /**
     * Deconstructor
     */
    ~pdu_align_impl();
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_ALIGN_IMPL_H */
