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

#include "pdu_split_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace pdu_utils {

pdu_split::sptr pdu_split::make(bool pass_empty_data)
{
    return gnuradio::make_block_sptr<pdu_split_impl>(pass_empty_data);
}

/*
 * The private constructor
 */
pdu_split_impl::pdu_split_impl(bool pass_empty_data)
    : gr::block("pdu_split", io_signature::make(0, 0, 0), io_signature::make(0, 0, 0)),
      d_pass_empty_data(pass_empty_data)
{
    message_port_register_in(PMTCONSTSTR__pdu_in());
    set_msg_handler(PMTCONSTSTR__pdu_in(),
                    [this](pmt::pmt_t msg) { this->handle_pdu(msg); });
    message_port_register_out(PMTCONSTSTR__dict());
    message_port_register_out(PMTCONSTSTR__data());
}

/*
 * Our virtual destructor.
 */
pdu_split_impl::~pdu_split_impl() {}

void pdu_split_impl::handle_pdu(pmt::pmt_t pdu)
{

    // make sure PDU data is formed properly
    if (!(pmt::is_pair(pdu))) {
        GR_LOG_WARN(d_logger, "received unexpected PMT (non-pair)");
        return;
    }

    pmt::pmt_t meta = pmt::car(pdu);
    pmt::pmt_t v_data = pmt::cdr(pdu);

    if ((!pmt::equal(meta, pmt::PMT_NIL)) | d_pass_empty_data) {
        message_port_pub(PMTCONSTSTR__dict(), pmt::car(pdu));
    }

    if (pmt::is_uniform_vector(v_data)) {
        if (pmt::length(v_data) | d_pass_empty_data) {
            message_port_pub(PMTCONSTSTR__data(), pmt::cdr(pdu));
        }
    }
}

} /* namespace pdu_utils */
} /* namespace gr */
