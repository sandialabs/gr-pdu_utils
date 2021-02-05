/* -*- c++ -*- */
/*
 * Copyright 2018-2021 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "pdu_quadrature_demod_cf_impl.h"
#include <gnuradio/io_signature.h>
#include <gnuradio/math.h>
#include <pdu_utils/constants.h>

#include <inttypes.h>
#include <volk/volk.h>

namespace gr {
namespace pdu_utils {

pdu_quadrature_demod_cf::sptr pdu_quadrature_demod_cf::make(float sensitivity)
{
    return gnuradio::make_block_sptr<pdu_quadrature_demod_cf_impl>(sensitivity);
}

/*
 * The private constructor
 */
pdu_quadrature_demod_cf_impl::pdu_quadrature_demod_cf_impl(float sensitivity)
    : gr::block("pdu_quadrature_demod_cf",
                gr::io_signature::make(0, 0, 0),
                gr::io_signature::make(0, 0, 0)),
      d_sensitivity(sensitivity)
{
    message_port_register_in(PMTCONSTSTR__cpdus());
    message_port_register_out(PMTCONSTSTR__fpdus());
    set_msg_handler(PMTCONSTSTR__cpdus(),
                    [this](pmt::pmt_t msg) { this->handle_pdu(msg); });
}

/*
 * Our virtual destructor.
 */
pdu_quadrature_demod_cf_impl::~pdu_quadrature_demod_cf_impl() {}

void pdu_quadrature_demod_cf_impl::handle_pdu(pmt::pmt_t pdu)
{
    pmt::pmt_t samples = pmt::cdr(pdu);
    size_t burst_size;
    const gr_complex* burst =
        (const gr_complex*)pmt::c32vector_elements(samples, burst_size);
    // Subtract off 1 from burst size because we do an offset for the conjugate multiply.
    burst_size--;

    std::vector<gr_complex> tmp(burst_size);
    std::vector<float> out(burst_size);

    volk_32fc_x2_multiply_conjugate_32fc(&tmp[0], &burst[1], &burst[0], burst_size);
    for (int i = 0; i < burst_size; i++) {
        out[i] = d_sensitivity * gr::fast_atan2f(imag(tmp[i]), real(tmp[i]));
    }

    pmt::pmt_t pdu_vector = pmt::init_f32vector(burst_size, out);
    pmt::pmt_t out_msg = pmt::cons(pmt::car(pdu), pdu_vector);
    message_port_pub(PMTCONSTSTR__fpdus(), out_msg);
}

} /* namespace pdu_utils */
} /* namespace gr */
