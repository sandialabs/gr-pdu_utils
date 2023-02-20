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

#include "pdu_length_filter_impl.h"
#include <gnuradio/io_signature.h>
#include <boost/format.hpp>
namespace gr {
namespace pdu_utils {

pdu_length_filter::sptr pdu_length_filter::make(uint32_t length, bool drop_long)
{
    return gnuradio::make_block_sptr<pdu_length_filter_impl>(length, drop_long);
}

/*
 * The private constructor
 */
pdu_length_filter_impl::pdu_length_filter_impl(uint32_t length, bool drop_long)
    : gr::block(
          "pdu_length_filter", io_signature::make(0, 0, 0), io_signature::make(0, 0, 0)),
      d_length(length),
      d_drop_long(drop_long),
      d_n_passed(0),
      d_n_blocked(0)
{
    message_port_register_in(PMTCONSTSTR__pdu_in());
    set_msg_handler(PMTCONSTSTR__pdu_in(),
                    [this](pmt::pmt_t msg) { this->handle_pdu(msg); });
    message_port_register_out(PMTCONSTSTR__pdu_out());
}

/*
 * Our virtual destructor.
 */
pdu_length_filter_impl::~pdu_length_filter_impl() {}


/*
 * This goes here as print statements in the destructor are usually not shown
 */
bool pdu_length_filter_impl::stop()
{
    d_logger->info("PDU length filter blocked {} and passed {} PDUs",
                    d_n_blocked,d_n_passed);
    return true;
}


/*
 * This goes here as print statements in the destructor are usually not shown
 */
void pdu_length_filter_impl::handle_pdu(pmt::pmt_t pdu)
{
    // make sure PDU data is formed properly
    if (!(pmt::is_pair(pdu))) {
        d_logger->warn("received unexpected PMT (non-pair)");
        return;
    }

    if (!pmt::is_uniform_vector(pmt::cdr(pdu))) {
        d_logger->warn("received unexpected PMT (CDR not uniform vector)");
        return;
    }

    size_t vlen = pmt::length(pmt::cdr(pdu));
    if ((d_drop_long && (vlen > d_length)) || (!d_drop_long && (vlen < d_length))) {
        d_n_blocked++;
        return;
    }
    d_n_passed++;
    message_port_pub(PMTCONSTSTR__pdu_out(), pdu);
}

} /* namespace pdu_utils */
} /* namespace gr */
