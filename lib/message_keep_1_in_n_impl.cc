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

#include "message_keep_1_in_n_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace pdu_utils {

message_keep_1_in_n::sptr message_keep_1_in_n::make(uint32_t n)
{
    return gnuradio::get_initial_sptr(new message_keep_1_in_n_impl(n));
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

    message_port_register_in(PMTCONSTSTR__IN);
    set_msg_handler(PMTCONSTSTR__IN,
                    boost::bind(&message_keep_1_in_n_impl::handle_msg, this, _1));
    message_port_register_out(PMTCONSTSTR__OUT);
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
        message_port_pub(PMTCONSTSTR__OUT, msg);
        d_ctr = 0;
    }
}

void message_keep_1_in_n_impl::set_n(uint32_t n)
{
    gr::thread::scoped_lock l(d_setlock);

    if (n) {
        d_n = n;
    } else {
        GR_LOG_ERROR(d_logger, boost::format("n cannot be zero, not set (n=%d)") % d_n);
    }
}


} /* namespace pdu_utils */
} /* namespace gr */
