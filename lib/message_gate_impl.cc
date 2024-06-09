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

#include "message_gate_impl.h"
#include <gnuradio/io_signature.h>
#include <boost/format.hpp>
namespace gr {
namespace pdu_utils {

message_gate::sptr message_gate::make(bool enabled)
{
    return gnuradio::make_block_sptr<message_gate_impl>(enabled);
}

/*
 * The private constructor
 */
message_gate_impl::message_gate_impl(bool enabled)
    : gr::block("message_gate", io_signature::make(0, 0, 0), io_signature::make(0, 0, 0)),
      d_enabled(enabled),
      d_n_blocked(0),
      d_n_passed(0)
{
    message_port_register_in(PMTCONSTSTR__in());
    set_msg_handler(PMTCONSTSTR__in(), [this](pmt::pmt_t msg) { this->handle_msg(msg); });
    message_port_register_out(PMTCONSTSTR__out());
}

/*
 * Our virtual destructor.
 */
message_gate_impl::~message_gate_impl() {}


/*
 * This goes here as print statements in the destructor are usually not shown
 */
bool message_gate_impl::stop()
{
    d_logger->info("Message gate passed {} messages and blocked {} messages",
                    d_n_passed,d_n_blocked);
    return true;
}


void message_gate_impl::setup_rpc()
{
#ifdef GR_CTRLPORT

    add_rpc_variable(rpcbasic_sptr(
        new rpcbasic_register_get<message_gate, uint64_t>(alias(),
                                                          "cp_get_n_passed",
                                                          &message_gate::get_n_passed,
                                                          pmt::mp(0),
                                                          pmt::mp(10000),
                                                          pmt::mp(100),
                                                          "",
                                                          "Get number of messages passed",
                                                          RPC_PRIVLVL_MIN,
                                                          DISPTIME | DISPOPTSTRIP)));

    add_rpc_variable(rpcbasic_sptr(new rpcbasic_register_get<message_gate, uint64_t>(
        alias(),
        "cp_get_n_blocked",
        &message_gate::get_n_blocked,
        pmt::mp(0),
        pmt::mp(10000),
        pmt::mp(100),
        "",
        "Get number of messages blocked",
        RPC_PRIVLVL_MIN,
        DISPTIME | DISPOPTSTRIP)));

    add_rpc_variable(rpcbasic_sptr(
        new rpcbasic_register_get<message_gate, bool>(alias(),
                                                      "cp_get_enabled",
                                                      &message_gate::get_enabled,
                                                      pmt::mp(0),
                                                      pmt::mp(10000),
                                                      pmt::mp(100),
                                                      "",
                                                      "Get counter",
                                                      RPC_PRIVLVL_MIN,
                                                      DISPTIME | DISPOPTSTRIP)));

    add_rpc_variable(rpcbasic_sptr(
        new rpcbasic_register_set<message_gate, bool>(alias(),
                                                      "cp_set_enabled",
                                                      &message_gate::set_enabled,
                                                      pmt::mp(0),
                                                      pmt::mp(10000),
                                                      pmt::mp(100),
                                                      "",
                                                      "Set enabled",
                                                      RPC_PRIVLVL_MIN,
                                                      DISPNULL)));

#endif /* GR_CTRLPORT */
}


void message_gate_impl::handle_msg(pmt::pmt_t msg)
{
    gr::thread::scoped_lock l(d_setlock);

    if (d_enabled) {
        d_n_passed++;
        message_port_pub(PMTCONSTSTR__out(), msg);
    } else {
        d_n_blocked++;
    }
}


void message_gate_impl::reset(void)
{
    gr::thread::scoped_lock l(d_setlock);

    d_n_passed = 0;
    d_n_blocked = 0;
    d_enabled = true;
}

void message_gate_impl::set_enabled(bool enabled)
{
    gr::thread::scoped_lock l(d_setlock);

    d_enabled = enabled;
}

} /* namespace pdu_utils */
} /* namespace gr */
