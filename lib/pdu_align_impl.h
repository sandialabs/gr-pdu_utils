/* -*- c++ -*- */
/*
 * Copyright 2018 National Technology & Engineering Solutions of Sandia, LLC (NTESS).
 * Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government retains
 * certain rights in this software.
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
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
