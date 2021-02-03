/* -*- c++ -*- */
/*
 * Copyright 2018, 2019, 2020 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */


#ifndef INCLUDED_PDU_UTILS_PDU_BINARY_TOOLS_H
#define INCLUDED_PDU_UTILS_PDU_BINARY_TOOLS_H

#include <gnuradio/sync_block.h>
#include <pdu_utils/api.h>

namespace gr {
namespace pdu_utils {

/*!
 * \brief binary operations on uint8_t PDUs
 * \ingroup pdu_utils
 *
 * This block does not do byte-level operations, only binary, stored in uint8_t.
 *
 * Bit flip mode: Bitflips the uint8 vector portion of a pmt.
 * To NRZ mode: Takes 0/1 data in uint8_t vector and changes to -1/1
 * From NRZ mode: Takes -1/1 data in float vector and changes to 0/1
 * Slice mode: negative values map to 0, positive values map to 1
 * Endian Swap mode: read a bit stream and flip the bit order on 8 byte boundaries so that
 * the byte order is preserved, but each byte is endian-swapped
 * Manchester Encode: Encode each each positive bit as a high to low transittion, and each
 * low bit as a high to low transition.  Accepts both uint8_t and float data, outputs the
 * same data type. Manchester Decode: Transition from low to high decodes to a 0 and from
 * high to low as a 1.  Accepts both uint8_t and float data types, outputs uint8_t.
 */
class PDU_UTILS_API pdu_binary_tools : virtual public gr::sync_block
{
public:
    typedef boost::shared_ptr<pdu_binary_tools> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of pdu_utils::pdu_binary_tools.
     *
     * @param mode - operation mode #Modes
     */
    static sptr make(uint8_t mode);

    // Enum for mode setting in this block
    enum Modes {
        BIT_FLIP = 0,
        TO_NRZ,
        FROM_NRZ,
        SLICE,
        ENDIAN_SWAP8,
        MANCHESTER_ENCODE,
        MANCHESTER_DECODE
    };
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_BINARY_TOOLS_H */
