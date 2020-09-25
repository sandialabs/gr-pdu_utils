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

#include "message_emitter_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace pdu_utils {

message_emitter::sptr message_emitter::make(pmt::pmt_t msg)
{
    return gnuradio::get_initial_sptr(new message_emitter_impl(msg));
}

/*
 * The private constructor
 */
message_emitter_impl::message_emitter_impl(pmt::pmt_t msg)
    : gr::block(
          "message_emitter", io_signature::make(0, 0, 0), io_signature::make(0, 0, 0)),
      d_msg(msg),
      d_n_msgs(0)
{
    message_port_register_in(PMTCONSTSTR__EMIT);
    set_msg_handler(PMTCONSTSTR__EMIT,
                    boost::bind(&message_emitter_impl::emit, this, _1));
    message_port_register_out(PMTCONSTSTR__MSG);
}

/*
 * Our virtual destructor.
 */
message_emitter_impl::~message_emitter_impl() {}


/*
 * This goes here as print statements in the destructor are usually not shown
 */
bool message_emitter_impl::stop()
{
    GR_LOG_INFO(d_logger, boost::format("Message emitter sent %d messages") % d_n_msgs);
    
    return true;
}


void message_emitter_impl::setup_rpc()
{
#ifdef GR_CTRLPORT

    rpcbasic_sptr(new rpcbasic_register_handler<message_emitter>(
        alias(), "emit", "", "Configure settings", RPC_PRIVLVL_MIN, DISPNULL));

#endif /* GR_CTRLPORT */
}


/*
 * The only thing this block does... emit messages when emit() is called -
 * this can be the defined message or a provided one
 */
void message_emitter_impl::emit()
{
    d_n_msgs++;
    message_port_pub(PMTCONSTSTR__MSG, d_msg);
}

void message_emitter_impl::emit(pmt::pmt_t msg)
{
    gr::thread::scoped_lock l(d_setlock);

    d_n_msgs++;
    /* if the emitter argument is PMT_NIL, send the stored message*/
    if (pmt::eqv(msg, pmt::PMT_NIL)) {
        message_port_pub(PMTCONSTSTR__MSG, d_msg);
    } else {
        message_port_pub(PMTCONSTSTR__MSG, msg);
    }
}

void message_emitter_impl::set_msg(pmt::pmt_t msg)
{
    gr::thread::scoped_lock l(d_setlock);

    d_msg = msg;
}

} /* namespace pdu_utils */
} /* namespace gr */
