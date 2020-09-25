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

#include "message_counter_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace pdu_utils {

message_counter::sptr message_counter::make(std::string name)
{
    return gnuradio::get_initial_sptr(new message_counter_impl(name));
}

/**
 * Constructor
 *
 * @param name - name of counter
 */
message_counter_impl::message_counter_impl(std::string name)
    : gr::block(
          "message_counter", io_signature::make(0, 0, 0), io_signature::make(0, 0, 0)),
      d_name(name),
      d_ctr(0)
{
    message_port_register_in(PMTCONSTSTR__MSG);
    set_msg_handler(PMTCONSTSTR__MSG,
                    boost::bind(&message_counter_impl::handle_msg, this, _1));
}

/**
 * deconstructor
 */
message_counter_impl::~message_counter_impl() {}


/*
 * This goes here as print statements in the destructor are usually not
 * shown - message is essential to blocks function and will be printed
 * with or without GR_LOG
 */
bool message_counter_impl::stop()
{
#ifdef ENABLE_GR_LOG
    GR_LOG_INFO(d_logger,
                boost::format("Message Counter '%s' got %d messages") % d_name % d_ctr);
#else
    std::cout << alias() << " :INFO: Message counter " << d_name << " got " << d_ctr
              << " messages" << std::endl;
#endif

    return true;
}


void message_counter_impl::setup_rpc()
{
#ifdef GR_CTRLPORT

    add_rpc_variable(rpcbasic_sptr(
        new rpcbasic_register_get<message_counter, uint64_t>(alias(),
                                                             "cp_get_counter",
                                                             &message_counter::get_ctr,
                                                             pmt::mp(0),
                                                             pmt::mp(10000),
                                                             pmt::mp(100),
                                                             "",
                                                             "Get counter",
                                                             RPC_PRIVLVL_MIN,
                                                             DISPTIME | DISPOPTSTRIP)));

#endif /* GR_CTRLPORT */
}

/**
 * message handler
 *
 * @param msg - incoming message
 */
void message_counter_impl::handle_msg(pmt::pmt_t msg) { d_ctr++; }

} /* namespace pdu_utils */
} /* namespace gr */
