/* -*- c++ -*- */
/*
 * Copyright 2018, 2019, 2020 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_PDU_UTILS_PDU_ALIGN_H
#define INCLUDED_PDU_UTILS_PDU_ALIGN_H

#include <gnuradio/block.h>
#include <pdu_utils/api.h>
#include <pdu_utils/constants.h>

namespace gr {
namespace pdu_utils {

/*!
 * \brief Aligns to a sync word
 * \ingroup pdu_utils
 *
 * Takes in unpacked u8vector PDUs and seaches for a syncword. If found, the
 * block outputs the entire PDU except for the syncword (with a programmable
 * offset). If not found, the PDU is dropped.
 *
 * The syncwords field is a string that takes a list of comma separated binary
 * numbers. For example, if using the 32-bit syncword 0xDEADBEEF, you would
 * enter it in binary (11011110101011011011111011101111) with an offset of 0.
 * The PDU will be truncated up through the syncword. If the offset is set to
 * -16, 0xDEAD is truncated, but 0xBEEF remains.
 *
 * The threshold is the number of bit errors allowed to still count as a match.
 *
 */
class PDU_UTILS_API pdu_align : virtual public gr::block
{
public:
    typedef boost::shared_ptr<pdu_align> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of pdu_utils::pdu_align.
     *
     * @param syncwords - binary syncword to search for
     * @param threshold - number of bit errors allowed
     * @param offset - bit offset to pass through
     * @param mode - mode to operate when sync is not found
     *  ALIGN_DROP(default) - drop pdu
     *  ALIGN_FORWARD - forward pdu
     *  ALIGN_EMPTY - emit empty pdu
     */
    static sptr
    make(std::string syncwords, int threshold, int offset, align_modes mode = ALIGN_DROP);
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_ALIGN_H */
