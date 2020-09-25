/* -*- c++ -*- */
/*
 * Copyright 2018, 2019, 2020 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "pdu_complex_to_mag2_impl.h"
#include <gnuradio/io_signature.h>

#include <volk/volk.h>

namespace gr {
namespace pdu_utils {

pdu_complex_to_mag2::sptr pdu_complex_to_mag2::make()
{
    return gnuradio::get_initial_sptr(new pdu_complex_to_mag2_impl());
}

/*
 * The private constructor
 */
pdu_complex_to_mag2_impl::pdu_complex_to_mag2_impl()
    : gr::block("pdu_complex_to_mag2",
                gr::io_signature::make(0, 0, 0),
                gr::io_signature::make(0, 0, 0))
{
    message_port_register_in(pmt::mp("cpdus"));
    message_port_register_out(pmt::mp("fpdus"));
    set_msg_handler(pmt::mp("cpdus"),
                    boost::bind(&pdu_complex_to_mag2_impl::handle_pdu, this, _1));
}

/*
 * Our virtual destructor.
 */
pdu_complex_to_mag2_impl::~pdu_complex_to_mag2_impl() {}


void pdu_complex_to_mag2_impl::handle_pdu(pmt::pmt_t pdu)
{
    pmt::pmt_t samples = pmt::cdr(pdu);
    size_t burst_size;
    const gr_complex* burst =
        (const gr_complex*)pmt::c32vector_elements(samples, burst_size);

    std::vector<float> out(burst_size);

    volk_32fc_magnitude_squared_32f(&out[0], &burst[0], burst_size);

    pmt::pmt_t pdu_vector = pmt::init_f32vector(burst_size, out);
    pmt::pmt_t out_msg = pmt::cons(pmt::car(pdu), pdu_vector);
    message_port_pub(pmt::mp("fpdus"), out_msg);
}


} /* namespace pdu_utils */
} /* namespace gr */
