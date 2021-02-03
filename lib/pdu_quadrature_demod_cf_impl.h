/* -*- c++ -*- */
/*
 * Copyright 2018, 2019, 2020 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_PDU_UTILS_PDU_QUADRATURE_DEMOD_CF_IMPL_H
#define INCLUDED_PDU_UTILS_PDU_QUADRATURE_DEMOD_CF_IMPL_H

#include <gnuradio/filter/fir_filter.h>
#include <pdu_utils/constants.h>
#include <pdu_utils/pdu_quadrature_demod_cf.h>

namespace gr {
namespace pdu_utils {

class pdu_quadrature_demod_cf_impl : public pdu_quadrature_demod_cf
{
private:
    float d_sensitivity;

public:
    /**
     * Constructor
     *
     * @param sensitivity -
     */
    pdu_quadrature_demod_cf_impl(float sensitivity);

    /**
     * Deconstructor
     */
    ~pdu_quadrature_demod_cf_impl();

    void handle_pdu(pmt::pmt_t pdu);
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_QUADRATURE_DEMOD_CF_IMPL_H */
