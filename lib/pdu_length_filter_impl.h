/* -*- c++ -*- */
/*
 * Copyright 2018-2021 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_PDU_UTILS_PDU_LENGTH_FILTER_IMPL_H
#define INCLUDED_PDU_UTILS_PDU_LENGTH_FILTER_IMPL_H

#include <gnuradio/pdu_utils/constants.h>
#include <gnuradio/pdu_utils/pdu_length_filter.h>

namespace gr {
namespace pdu_utils {

class pdu_length_filter_impl : public pdu_length_filter
{
private:
    uint32_t d_length;
    bool d_drop_long;
    uint64_t d_n_passed;
    uint64_t d_n_blocked;

    void handle_pdu(pmt::pmt_t pdu);

public:
    pdu_length_filter_impl(uint32_t length, bool drop_long);

    ~pdu_length_filter_impl() override;
    bool stop() override; // overloaded for print output
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_LENGTH_FILTER_IMPL_H */
