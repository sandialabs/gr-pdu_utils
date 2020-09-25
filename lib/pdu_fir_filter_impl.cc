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

#include "pdu_fir_filter_impl.h"
#include "volk/volk.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace pdu_utils {

pdu_fir_filter::sptr pdu_fir_filter::make(int decimation, const std::vector<float> taps)
{
    return gnuradio::get_initial_sptr(new pdu_fir_filter_impl(decimation, taps));
}

/*
 * The private constructor
 */
pdu_fir_filter_impl::pdu_fir_filter_impl(int decimation, const std::vector<float> taps)
    : gr::block("pdu_fir_filter",
                gr::io_signature::make(0, 0, 0),
                gr::io_signature::make(0, 0, 0)),
      d_fir_fff(1, taps),
      d_fir_ccf(1, taps),
      d_decimation(decimation)
{

    // to avoid memory access issues within the filterNdec function of gnuradio
    // pad the input at the front and back based on the volk alignemt
    d_pad = volk_get_alignment() / sizeof(float);

    message_port_register_in(PMTCONSTSTR__PDU_IN);
    message_port_register_out(PMTCONSTSTR__PDU_OUT);
    set_msg_handler(PMTCONSTSTR__PDU_IN,
                    boost::bind(&pdu_fir_filter_impl::handle_pdu, this, _1));
}

/*
 * Our virtual destructor.
 */
pdu_fir_filter_impl::~pdu_fir_filter_impl() {}

void pdu_fir_filter_impl::handle_pdu(pmt::pmt_t pdu)
{
    // make sure PDU data is formed properly
    if (!(pmt::is_pair(pdu))) {
        GR_LOG_NOTICE(d_logger, "received unexpected PMT (non-pair)");
        return;
    }

    pmt::pmt_t metadata = pmt::car(pdu);
    pmt::pmt_t pdu_data = pmt::cdr(pdu);

    if (!pmt::is_dict(metadata)) {
        GR_LOG_WARN(d_logger, "PDU metadata is not a dict, dropping");
        return;
    }

    // if there's a sample rate tag, update it
    if (pmt::dict_has_key(metadata, pmt::mp("sample_rate"))) {
        double sample_rate =
            pmt::to_double(pmt::dict_ref(metadata, pmt::mp("sample_rate"), pmt::PMT_NIL));
        sample_rate /= d_decimation;
        metadata = pmt::dict_delete(metadata, pmt::mp("sample_rate"));
        metadata = pmt::dict_add(
            metadata, pmt::mp("sample_rate"), pmt::from_double(sample_rate));
    }

    if (pmt::is_f32vector(pdu_data)) {
        const std::vector<float> d_in_p = pmt::f32vector_elements(pdu_data);
        if (d_in_p.size() <= d_fir_fff.ntaps()) {
         // not enough data to process
         return;
        }
        std::vector<float> d_in(d_in_p.size()+2*d_pad);
        d_in.insert(d_in.begin()+d_pad,d_in_p.begin(),d_in_p.end());


        std::vector<float> d_out;
        d_out.resize((d_in_p.size() - d_fir_fff.ntaps() + 1) / d_decimation);

        // do FIR filtering
        d_fir_fff.filterNdec(d_out.data(), d_in.data()+d_pad, d_out.size(), d_decimation);

        message_port_pub(
            PMTCONSTSTR__PDU_OUT,
            (pmt::cons(metadata, pmt::init_f32vector(d_out.size(), d_out.data()))));
    } else if (pmt::is_c32vector(pdu_data)) {

        const std::vector<gr_complex> d_in_p = pmt::c32vector_elements(pdu_data);
        if (d_in_p.size() <= d_fir_ccf.ntaps()) {
         // not enough data to process
         return;
        }
        std::vector<gr_complex> d_in(d_in_p.size()+2*d_pad);
        d_in.insert(d_in.begin()+d_pad,d_in_p.begin(),d_in_p.end());
#if 0
        if (d_tmp.size() < d_in.size() + 20) {
            d_tmp.resize(d_in.size() + 200);
            memset(&d_tmp[0], 0, sizeof(gr_complex) * 10);
            memset(&d_tmp[d_in.size() - 10], 0, sizeof(gr_complex) * 10);
        }
        memcpy(&d_tmp[0] + 10, &d_in[0], sizeof(gr_complex) * d_in.size());
#endif
        std::vector<gr_complex> d_out;
        d_out.resize((d_in_p.size() - d_fir_ccf.ntaps() + 1) / d_decimation);

        // do FIR filtering
        d_fir_ccf.filterNdec(d_out.data(), d_in.data() + d_pad, d_out.size(), d_decimation);

        message_port_pub(
            PMTCONSTSTR__PDU_OUT,
            (pmt::cons(metadata, pmt::init_c32vector(d_out.size(), d_out.data()))));
    } else if (pmt::is_u8vector(pdu_data)) {

        const std::vector<uint8_t> d_in_byte = pmt::u8vector_elements(pdu_data);
        if (d_in_byte.size() <= d_fir_fff.ntaps()) {
         // not enough data to process
         return;
	}
        std::vector<float> d_in(d_in_byte.size()+2*d_pad);
        d_in.insert(d_in.begin()+d_pad,d_in_byte.begin(),d_in_byte.end());

        std::vector<float> d_out;
        d_out.resize((d_in_byte.size() - d_fir_fff.ntaps() + 1) / d_decimation);

        // do FIR filtering
        d_fir_fff.filterNdec(d_out.data(), d_in.data() + d_pad, d_out.size(), d_decimation);

        std::vector<uint8_t> d_out_byte(d_out.begin(), d_out.end());
        message_port_pub(
            PMTCONSTSTR__PDU_OUT,
            (pmt::cons(metadata,
                       pmt::init_u8vector(d_out_byte.size(), d_out_byte.data()))));
    } else {
        GR_LOG_WARN(d_logger, "PMT is not a byte, float, or complex PDU, dropping");
        return;
    }
}

} /* namespace pdu_utils */
} /* namespace gr */