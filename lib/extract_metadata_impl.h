/* -*- c++ -*- */
/*
 * Copyright 2018, 2019, 2020 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
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
