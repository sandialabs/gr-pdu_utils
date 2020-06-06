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

#ifndef INCLUDED_PDU_UTILS_EXTRACT_METADATA_IMPL_H
#define INCLUDED_PDU_UTILS_EXTRACT_METADATA_IMPL_H

#include <pdu_utils/constants.h>
#include <pdu_utils/extract_metadata.h>

namespace gr {
namespace pdu_utils {

class extract_metadata_impl : public extract_metadata
{
private:
    pmt::pmt_t d_key;
    float d_scale;
    float d_offset;

public:
    /**
     * Constructor
     *
     * @param key - dict key to look for
     * @param scale - scale factor to apply to number values
     * @param offset - offset to apply to number values.
     */
    extract_metadata_impl(pmt::pmt_t key, float scale, float offset);

    /**
     * Deconstructor
     */
    ~extract_metadata_impl();

    void handle_msg(pmt::pmt_t);

    /**
     * Set dict key to look for
     *
     * @param key - pmt key
     */
    void set_key(pmt::pmt_t key);

    /**
     * Set scale factor for number values
     *
     * @param scale - scale factor
     */
    void set_scale(float scale);

    /**
     * Set Offset for number values
     *
     * @param offset - offset value
     */
    void set_offset(float offset);

    /**
     * Returns current key value
     *
     * @return pmt::pmt_t
     */
    pmt::pmt_t get_key(void) { return d_key; }

    /**
     * Returns current scale value
     *
     * @return float
     */
    float get_scale(void) { return d_scale; }

    /**
     * Returns current offset value
     *
     * @return float
     */
    float get_offset(void) { return d_offset; }
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_EXTRACT_METADATA_IMPL_H */
