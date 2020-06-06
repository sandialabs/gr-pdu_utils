/* -*- c++ -*- */
/*
 * Copyright 2018 National Technology & Engineering Solutions of Sandia, LLC (NTESS).
 * Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government retains
 * certain rights in this software.
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "msg_drop_random_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace pdu_utils {

msg_drop_random::sptr msg_drop_random::make(float p_drop, uint64_t seed)
{
    return gnuradio::get_initial_sptr(new msg_drop_random_impl(p_drop, seed));
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
    message_port_register_in(PMTCONSTSTR__PDU_IN);
    set_msg_handler(PMTCONSTSTR__PDU_IN,
                    boost::bind(&msg_drop_random_impl::handle_msg, this, _1));
    message_port_register_out(PMTCONSTSTR__PDU_OUT);
}

/*
 * Our virtual destructor.
 */
msg_drop_random_impl::~msg_drop_random_impl() {}

bool msg_drop_random_impl::stop(void)
{
    GR_LOG_INFO(d_logger,
                boost::format("Dropped %s messages, passed %d (%d total)") % d_drop_ctr %
                    d_pass_ctr % d_msg_ctr);
    return true;
}


void msg_drop_random_impl::handle_msg(pmt::pmt_t msg)
{
    gr::thread::scoped_lock l(d_setlock);

    d_msg_ctr++;
    if (d_rng.ran1() > d_p_drop) {
        d_pass_ctr++;
        message_port_pub(PMTCONSTSTR__PDU_OUT, msg);
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
