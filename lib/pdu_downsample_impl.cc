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

#include "pdu_downsample_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace pdu_utils {

pdu_downsample::sptr pdu_downsample::make(int decimation, int phase)
{
    return gnuradio::get_initial_sptr(new pdu_downsample_impl(decimation, phase));
}

/*
 * The private constructor
 */
pdu_downsample_impl::pdu_downsample_impl(int decimation, int phase)
    : gr::sync_block("pdu_downsample",
                     gr::io_signature::make(0, 0, 0),
                     gr::io_signature::make(0, 0, 0)),
      d_decimation(decimation),
      d_phase(phase)
{

    // register message ports
    message_port_register_in(pmt::intern("pdu_in"));
    message_port_register_out(pmt::intern("pdu_out"));
    set_msg_handler(pmt::intern("pdu_in"),
                    boost::bind(&pdu_downsample_impl::handle_msg, this, _1));
}

/*
 * Our virtual destructor.
 */
pdu_downsample_impl::~pdu_downsample_impl() {}

void pdu_downsample_impl::handle_msg(pmt::pmt_t pdu)
{
    // make sure PDU data is formed properly
    if (!(pmt::is_pair(pdu))) {
        GR_LOG_NOTICE(d_logger, "received unexpected PMT (non-pair)");
        return;
    }

    pmt::pmt_t meta = pmt::car(pdu);
    pmt::pmt_t v_data = pmt::cdr(pdu);

    // is it float type?
    if (pmt::is_f32vector(v_data)) {
        std::vector<float> in_data = pmt::f32vector_elements(v_data);
        std::vector<float> out_data;

        for (size_t i = d_phase; i < in_data.size(); i += d_decimation) {
            out_data.push_back(in_data[i]);
        }

        // publish the new pdu
        message_port_pub(
            pmt::intern("pdu_out"),
            (pmt::cons(meta, pmt::init_f32vector(out_data.size(), out_data))));
    }

    // is it u8 type?
    else if (pmt::is_u8vector(v_data)) {
        std::vector<uint8_t> in_data = pmt::u8vector_elements(v_data);
        std::vector<uint8_t> out_data;

        for (size_t i = d_phase; i < in_data.size(); i += d_decimation) {
            out_data.push_back(in_data[i]);
        }

        // publish the new pdu
        message_port_pub(
            pmt::intern("pdu_out"),
            (pmt::cons(meta, pmt::init_u8vector(out_data.size(), out_data))));
    }
    // give up
    else {
        GR_LOG_NOTICE(d_logger, "pdu downsample block can't handle this datatype");
    }
}

int pdu_downsample_impl::work(int noutput_items,
                              gr_vector_const_void_star& input_items,
                              gr_vector_void_star& output_items)
{
    return noutput_items;
}

} /* namespace pdu_utils */
} /* namespace gr */
