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

#include "pdu_burst_combiner_impl.h"
#include <gnuradio/io_signature.h>

#include <gnuradio/blocks/pdu.h>
#include <cmath>


namespace gr {
namespace pdu_utils {

pdu_burst_combiner::sptr pdu_burst_combiner::make()
{
    return gnuradio::get_initial_sptr(new pdu_burst_combiner_impl());
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

    message_port_register_in(PMTCONSTSTR__PDU_IN);
    set_msg_handler(PMTCONSTSTR__PDU_IN,
                    boost::bind(&pdu_burst_combiner_impl::handle_pdu, this, _1));
    message_port_register_out(PMTCONSTSTR__PDU_OUT);
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
        GR_LOG_NOTICE(d_logger, "received unexpected PMT (non-pair)");
        return;
    }

    /* code */
    pmt::pmt_t meta = pmt::car(pdu);
    pmt::pmt_t v_data = pmt::cdr(pdu);


    if (!(is_dict(meta) && pmt::is_c32vector(v_data))) {
        GR_LOG_WARN(d_logger, "PMT is not a complex PDU, dropping");
        return;
    }

    uint32_t v_len = pmt::length(v_data);

    if (v_len == 0) {
        GR_LOG_WARN(d_logger, "Zero length PDU, ignoring");
        return;
    }


    // get the burst index, or a (0, 0) tuple if not present (regular burst)
    pmt::pmt_t burst_index =
        pmt::dict_ref(meta,
                      pmt::intern("burst_index"),
                      pmt::cons(pmt::from_uint64(0), pmt::from_uint64(0)));
    uint64_t x = pmt::to_uint64(pmt::car(burst_index));
    uint64_t y = pmt::to_uint64(pmt::cdr(burst_index));

    if (d_burst_count != 0) {
        // we are in the middle of processing bursts...
        if (x > y) {
            // error...bad
            GR_LOG_ALERT(
                d_logger,
                boost::format("Error processing PDU, burst_index metadata invalid (%)") %
                    burst_index);
            GR_LOG_ERROR(d_logger, "resetting state and dropping PDU");
            reset_state();
        } else if (x < y) {
            // not the last burst in the string
            const std::vector<gr_complex> d_in = pmt::c32vector_elements(v_data);
            d_data.insert(d_data.end(), d_in.begin(), d_in.end());
            GR_LOG_DEBUG(d_logger,
                         boost::format("saving PDU %d / %d! size is %d") % x % y %
                             d_data.size());

        } else {
            // done with the burst, add new stuff and send it
            const std::vector<gr_complex> d_in = pmt::c32vector_elements(v_data);
            d_data.insert(d_data.end(), d_in.begin(), d_in.end());
            GR_LOG_DEBUG(d_logger,
                         boost::format("saving PDU %d / %d! size is %d") % x % y %
                             d_data.size());

            message_port_pub(
                PMTCONSTSTR__PDU_OUT,
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
                GR_LOG_DEBUG(d_logger,
                             boost::format("saving PDU %d / %d! size is %d") % x % y %
                                 d_data.size());

                d_burst_count = y;
                d_burst0_metadata = meta;

            } else {
                GR_LOG_ERROR(d_logger,
                             "Error processing PDU, first burst does not have index = 1");
                GR_LOG_ERROR(d_logger, "resetting state and dropping PDU");
                reset_state();
            }
        } else {
            // this is the first and only burst, send it as is
            message_port_pub(PMTCONSTSTR__PDU_OUT, pdu);
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
