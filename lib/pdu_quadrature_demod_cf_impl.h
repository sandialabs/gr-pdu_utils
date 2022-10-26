/* -*- c++ -*- */
/*
 * Copyright 2018-2021 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_PDU_UTILS_PDU_QUADRATURE_DEMOD_CF_IMPL_H
#define INCLUDED_PDU_UTILS_PDU_QUADRATURE_DEMOD_CF_IMPL_H

#include <gnuradio/filter/fir_filter.h>
#include <gnuradio/pdu_utils/pdu_quadrature_demod_cf.h>

namespace gr {
namespace pdu_utils {

class pdu_quadrature_demod_cf_impl : public pdu_quadrature_demod_cf
{
private:
    float d_sensitivity;

    void handle_pdu(pmt::pmt_t pdu);

public:
    pdu_quadrature_demod_cf_impl(float sensitivity);

    ~pdu_quadrature_demod_cf_impl() override;
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_QUADRATURE_DEMOD_CF_IMPL_H */
