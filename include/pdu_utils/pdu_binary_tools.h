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
    enum Modes { BIT_FLIP = 0, TO_NRZ, FROM_NRZ, SLICE, ENDIAN_SWAP8 };
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_BINARY_TOOLS_H */
