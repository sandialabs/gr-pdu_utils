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

#include "message_keep_1_in_n_impl.h"
#include <gnuradio/io_signature.h>
#include <boost/format.hpp>
namespace gr {
namespace pdu_utils {

message_keep_1_in_n::sptr message_keep_1_in_n::make(uint32_t n)
{
    return gnuradio::make_block_sptr<message_keep_1_in_n_impl>(n);
}

/*
 * The private constructor
 */
message_keep_1_in_n_impl::message_keep_1_in_n_impl(uint32_t n)
    : gr::block("message_keep_1_in_n",
                io_signature::make(0, 0, 0),
                io_signature::make(0, 0, 0)),
      d_ctr(0)
{
    if (n) {
        set_n(n);
    } else {
        throw std::invalid_argument("Message Keep 1 in N: n cannot be zero!");
    }

    message_port_register_in(PMTCONSTSTR__in());
    set_msg_handler(PMTCONSTSTR__in(), [this](pmt::pmt_t msg) { this->handle_msg(msg); });
    message_port_register_out(PMTCONSTSTR__out());
}

/*
 * Our virtual destructor.
 */
message_keep_1_in_n_impl::~message_keep_1_in_n_impl() {}

/*
 * Message handler
 */
void message_keep_1_in_n_impl::handle_msg(pmt::pmt_t msg)
{
    gr::thread::scoped_lock l(d_setlock);

    d_ctr++;
    if (d_ctr >= d_n) {
        message_port_pub(PMTCONSTSTR__out(), msg);
        d_ctr = 0;
    }
}

void message_keep_1_in_n_impl::set_n(uint32_t n)
{
    gr::thread::scoped_lock l(d_setlock);

    if (n) {
        d_n = n;
    } else {
        d_logger->error("n cannot be zero, not set (n={})",d_n);
    }
}


} /* namespace pdu_utils */
} /* namespace gr */
