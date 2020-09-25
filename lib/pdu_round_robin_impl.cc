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

#include "pdu_round_robin_impl.h"
#include "pdu_utils/constants.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace pdu_utils {

pdu_round_robin::sptr pdu_round_robin::make(int num_outputs)
{
    return gnuradio::get_initial_sptr(new pdu_round_robin_impl(num_outputs));
}

/*
 * The private constructor
 */
pdu_round_robin_impl::pdu_round_robin_impl(int num_outputs)
    : gr::block("pdu_round_robin",
                gr::io_signature::make(0, 0, 0),
                gr::io_signature::make(0, 0, 0)),
      d_num_outputs(num_outputs),
      d_counter(0)
{
    // inputs
    message_port_register_in(PMTCONSTSTR__PDU_IN);
    set_msg_handler(PMTCONSTSTR__PDU_IN,
                    boost::bind(&pdu_round_robin_impl::pdu_handler, this, _1));

    // outputs
    for (size_t i = 0; i < d_num_outputs; i++) {
        std::stringstream s;
        s << "pdu_out_" << i;
        d_output_ports.push_back(pmt::mp(s.str()));
        message_port_register_out(d_output_ports[i]);
    }
}

/*
 * Our virtual destructor.
 */
pdu_round_robin_impl::~pdu_round_robin_impl() {}

void pdu_round_robin_impl::pdu_handler(pmt::pmt_t pdu)
{
    message_port_pub(d_output_ports[d_counter], pdu);

    d_counter = (d_counter + 1) % d_num_outputs;
}

} /* namespace pdu_utils */
} /* namespace gr */
