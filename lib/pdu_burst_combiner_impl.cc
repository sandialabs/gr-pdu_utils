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

#include "pdu_burst_combiner_impl.h"
#include "gnuradio/pdu_utils/constants.h"
#include <gnuradio/io_signature.h>
#include <boost/format.hpp>
#include <cmath>


namespace gr {
namespace pdu_utils {

pdu_burst_combiner::sptr pdu_burst_combiner::make()
{
    return gnuradio::make_block_sptr<pdu_burst_combiner_impl>();
}

/*
 * The private constructor
 */
pdu_burst_combiner_impl::pdu_burst_combiner_impl()
    : gr::block("pdu_burst_combiner",
                gr::io_signature::make(0, 0, 0),
                gr::io_signature::make(0, 0, 0))
{

    reset_state();

    message_port_register_in(PMTCONSTSTR__pdu_in());
    set_msg_handler(PMTCONSTSTR__pdu_in(),
                    [this](pmt::pmt_t msg) { this->handle_pdu(msg); });
    message_port_register_out(PMTCONSTSTR__pdu_out());
}

/*
 * Our virtual destructor.
 */
pdu_burst_combiner_impl::~pdu_burst_combiner_impl() {}


/*
 * the main function, where everything happens
 */
void pdu_burst_combiner_impl::handle_pdu(pmt::pmt_t pdu)
{
    // make sure PDU data is formed properly
    if (!(pmt::is_pair(pdu))) {
        d_logger->notice("received unexpected PMT (non-pair)");
        return;
    }

    /* code */
    pmt::pmt_t meta = pmt::car(pdu);
    pmt::pmt_t v_data = pmt::cdr(pdu);


    if (!(is_dict(meta) && pmt::is_c32vector(v_data))) {
        d_logger->warn("PMT is not a complex PDU, dropping");
        return;
    }

    uint32_t v_len = pmt::length(v_data);

    if (v_len == 0) {
        d_logger->warn("Zero length PDU, ignoring");
        return;
    }


    // get the burst index, or a (0, 0) tuple if not present (regular burst)
    pmt::pmt_t burst_index =
        pmt::dict_ref(meta,
                      PMTCONSTSTR__burst_index(),
                      pmt::cons(pmt::from_uint64(0), pmt::from_uint64(0)));
    uint64_t x = pmt::to_uint64(pmt::car(burst_index));
    uint64_t y = pmt::to_uint64(pmt::cdr(burst_index));

    if (d_burst_count != 0) {
        // we are in the middle of processing bursts...
        if (x > y) {
            // error...bad
            std::ostringstream msg;
            msg << boost::format("Error processing PDU, burst_index metadata invalid (%)") % burst_index;
            d_logger->alert(msg.str());
            d_logger->error("resetting state and dropping PDU");
            reset_state();
        } else if (x < y) {
            // not the last burst in the string
            const std::vector<gr_complex> d_in = pmt::c32vector_elements(v_data);
            d_data.insert(d_data.end(), d_in.begin(), d_in.end());
            d_logger->debug("saving PDU {} / {}! size is {}",x,y,
                             d_data.size());

        } else {
            // done with the burst, add new stuff and send it
            const std::vector<gr_complex> d_in = pmt::c32vector_elements(v_data);
            d_data.insert(d_data.end(), d_in.begin(), d_in.end());
            d_logger->debug("saving PDU {} / {}! size is {}",x,y,
                             d_data.size());

            message_port_pub(
                PMTCONSTSTR__pdu_out(),
                pmt::cons(
                    d_burst0_metadata,
                    pmt::init_c32vector(d_data.size(), (const gr_complex*)&d_data[0])));
            reset_state();
        }
    } else {
        // this is the first burst
        if (y > 1) {
            // and there are multiple bursts
            if (x == 1) {
                reset_state();
                const std::vector<gr_complex> d_in = pmt::c32vector_elements(v_data);
                d_data.insert(d_data.end(), d_in.begin(), d_in.end());
                d_logger->debug("saving PDU {} / {}! size is {}",x,y,
                                 d_data.size());

                d_burst_count = y;
                d_burst0_metadata = meta;

            } else {
                d_logger->error("Error processing PDU, first burst does not have index = 1");
                d_logger->error("resetting state and dropping PDU");
                reset_state();
            }
        } else {
            // this is the first and only burst, send it as is
            message_port_pub(PMTCONSTSTR__pdu_out(), pdu);
            reset_state();
        }
    }
}


/*
 * reset the state of the block
 */
void pdu_burst_combiner_impl::reset_state(void)
{
    d_burst_count = 0;
    d_burst0_metadata = pmt::PMT_NIL;
    d_data.clear();
}

} /* namespace pdu_utils */
} /* namespace gr */
