/* -*- c++ -*- */
/*
 * Copyright 2018-2021 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_PDU_UTILS_PDU_GMSK_FC_IMPL_H
#define INCLUDED_PDU_UTILS_PDU_GMSK_FC_IMPL_H

#include <gnuradio/filter/fir_filter.h>
#include <gnuradio/pdu_utils/pdu_gmsk_fc.h>

namespace gr {
namespace pdu_utils {

class pdu_gmsk_fc_impl : public pdu_gmsk_fc
{
private:
    float d_sensitivity;
    float d_phase;
    std::vector<float> d_taps;
    filter::kernel::fir_filter_fff* d_fir;
    std::vector<float> d_log_ramp;

    void handle_pdu(pmt::pmt_t pdu);

public:
    pdu_gmsk_fc_impl(float sensitivity, const std::vector<float> taps);

    ~pdu_gmsk_fc_impl() override;

    void set_sensitivity(float sensitivity) override;
    void set_taps(std::vector<float> taps) override;
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_GMSK_FC_IMPL_H */
