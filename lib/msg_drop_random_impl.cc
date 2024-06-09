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

#include "msg_drop_random_impl.h"
#include <gnuradio/io_signature.h>
#include <boost/format.hpp>
namespace gr {
namespace pdu_utils {

msg_drop_random::sptr msg_drop_random::make(float p_drop, uint64_t seed)
{
    return gnuradio::make_block_sptr<msg_drop_random_impl>(p_drop, seed);
}

/*
 * The private constructor
 */
msg_drop_random_impl::msg_drop_random_impl(float p_drop, uint64_t seed)
    : gr::block(
          "msg_drop_random", io_signature::make(0, 0, 0), io_signature::make(0, 0, 0)),
      d_msg_ctr(0),
      d_pass_ctr(0),
      d_drop_ctr(0),
      d_p_drop(p_drop),
      d_rng(seed)
{
    message_port_register_in(PMTCONSTSTR__pdu_in());
    set_msg_handler(PMTCONSTSTR__pdu_in(),
                    [this](pmt::pmt_t msg) { this->handle_msg(msg); });
    message_port_register_out(PMTCONSTSTR__pdu_out());
}

/*
 * Our virtual destructor.
 */
msg_drop_random_impl::~msg_drop_random_impl() {}

bool msg_drop_random_impl::stop(void)
{
    d_logger->info("Dropped {} messages, passed {} ({} total)",d_drop_ctr,
                    d_pass_ctr,d_msg_ctr);
    return true;
}


void msg_drop_random_impl::handle_msg(pmt::pmt_t msg)
{
    gr::thread::scoped_lock l(d_setlock);

    d_msg_ctr++;
    if (d_rng.ran1() > d_p_drop) {
        d_pass_ctr++;
        message_port_pub(PMTCONSTSTR__pdu_out(), msg);
    } else {
        d_drop_ctr++;
    }
}

void msg_drop_random_impl::set_prob_drop(float p)
{
    gr::thread::scoped_lock l(d_setlock);

    d_p_drop = p;
}

void msg_drop_random_impl::reset(void)
{
    gr::thread::scoped_lock l(d_setlock);

    d_msg_ctr = 0;
    d_pass_ctr = 0;
    d_drop_ctr = 0;
}

} /* namespace pdu_utils */
} /* namespace gr */
