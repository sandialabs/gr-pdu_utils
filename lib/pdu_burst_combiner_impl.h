/* -*- c++ -*- */
/*
 * Copyright 2018-2021 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_PDU_UTILS_PDU_BURST_COMBINER_IMPL_H
#define INCLUDED_PDU_UTILS_PDU_BURST_COMBINER_IMPL_H

#include <pdu_utils/constants.h>
#include <pdu_utils/pdu_burst_combiner.h>

namespace gr {
namespace pdu_utils {

class pdu_burst_combiner_impl : public pdu_burst_combiner
{
private:
    std::vector<gr_complex> d_data;
    pmt::pmt_t d_burst0_metadata;
    uint32_t d_burst_count;

    void reset_state(void);

public:
    /**
     * Constructor
     */
    pdu_burst_combiner_impl();

    /**
     * Deconstructor
     */
    ~pdu_burst_combiner_impl();

    void handle_pdu(pmt::pmt_t pdu);
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_BURST_COMBINER_IMPL_H */
