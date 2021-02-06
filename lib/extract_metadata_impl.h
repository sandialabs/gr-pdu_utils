/* -*- c++ -*- */
/*
 * Copyright 2018-2021 National Technology & Engineering Solutions of Sandia, LLC
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

    /*!
     * \brief Message handler for input messages
     *
     * \param msg Dict PMT or PDU message passed from the scheduler's message handling.
     */
    void handle_msg(pmt::pmt_t msg);

public:
    extract_metadata_impl(pmt::pmt_t key, float scale, float offset);

    ~extract_metadata_impl() override;

    void set_key(pmt::pmt_t key) override;
    void set_scale(float scale) override;
    void set_offset(float offset) override;
    pmt::pmt_t get_key(void) override { return d_key; }
    float get_scale(void) override { return d_scale; }
    float get_offset(void) override { return d_offset; }
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_EXTRACT_METADATA_IMPL_H */
