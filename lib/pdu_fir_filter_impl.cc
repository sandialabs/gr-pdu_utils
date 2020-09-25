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

pdu_fir_filter::sptr pdu_fir_filter::make(int decimation, const std::vector<float> taps,
                                          bool pad_for_filter)
{
    return gnuradio::get_initial_sptr
      (new pdu_fir_filter_impl(decimation, taps, pad_for_filter));
}

/*
 * The private constructor
 */
pdu_fir_filter_impl::pdu_fir_filter_impl(int decimation, const std::vector<float> taps,
                                         bool pad_for_filter)
    : gr::block("pdu_fir_filter",
                gr::io_signature::make(0, 0, 0),
                gr::io_signature::make(0, 0, 0)),
      d_fir_fff(1, taps),
      d_fir_ccf(1, taps),
      d_decimation(decimation),
      d_pad_for_filter(pad_for_filter)
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
        double input_sample_rate = sample_rate;
        sample_rate /= d_decimation;
        metadata = pmt::dict_delete(metadata, pmt::mp("sample_rate"));
        metadata = pmt::dict_add(
            metadata, pmt::mp("sample_rate"), pmt::from_double(sample_rate));

        // if there's a "start_time" tag, update it (if we are not padding)
        if (!d_pad_for_filter && pmt::dict_has_key(metadata, pmt::mp("start_time"))) {
          double start_time =
            pmt::to_double(pmt::dict_ref(metadata, pmt::mp("start_time"), pmt::PMT_NIL));
          start_time += double(d_fir_fff.ntaps()>>1)/input_sample_rate;
          metadata = pmt::dict_delete(metadata, pmt::mp("start_time"));
          metadata =
            pmt::dict_add(metadata, pmt::mp("start_time"), pmt::from_double(start_time));
        }
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

        if (d_pad_for_filter)
        {
            size_t ntap = d_fir_fff.ntaps();
            size_t htap = ntap>>1;
            size_t rtap = ntap-htap; //in case ntap is odd
            size_t nin = d_in_p.size();
            size_t nbuf = nin + ntap;
            size_t nout = ((int)nin / d_decimation) + 1;
            if (d_ftmp.size() < nbuf)
            {
                d_ftmp.resize(nbuf);
            }
            memset(&d_ftmp[0], 0, sizeof(gr_complex)*htap);
            memset(&d_ftmp[nin+htap], 0, sizeof(gr_complex)*rtap);
            memcpy(&d_ftmp[0+htap], &d_in_p[0], sizeof(gr_complex)*nin);
            d_out.resize(nout);
            // do FIR filtering
            d_fir_fff.filterNdec(d_out.data(), d_ftmp.data(), nout, d_decimation);
        }
        else
        {
            d_out.resize((d_in_p.size() - d_fir_fff.ntaps() + 1) / d_decimation);

            // do FIR filtering
            d_fir_fff.filterNdec(d_out.data(), d_in.data()+d_pad, d_out.size(), d_decimation);
        }

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

        if (d_pad_for_filter)
        {
            size_t ntap = d_fir_ccf.ntaps();
            size_t htap = ntap>>1;
            size_t rtap = ntap-htap; //in case ntap is odd
            size_t nin = d_in_p.size();
            size_t nbuf = nin + ntap;
            size_t nout = ((int)nin / d_decimation) + 1;
            if (d_ctmp.size() < (nbuf+20))
            {
                d_ctmp.resize(nbuf+20);
            }
            memset(&d_ctmp[0], 0, sizeof(gr_complex)*(htap+10));
            memset(&d_ctmp[nin+htap+10], 0, sizeof(gr_complex)*(rtap+10));
            memcpy(&d_ctmp[0+htap+10], &d_in_p[0], sizeof(gr_complex)*nin);
            d_out.resize(nout);
            // do FIR filtering
            d_fir_ccf.filterNdec(d_out.data(), d_ctmp.data()+10, nout, d_decimation);
        }
        else
        {
            d_out.resize((d_in_p.size() - d_fir_ccf.ntaps() + 1) / d_decimation);

            // do FIR filtering
            d_fir_ccf.filterNdec(d_out.data(), d_in.data() + d_pad, d_out.size(), d_decimation);
        }

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

        if (d_pad_for_filter)
        {
            size_t ntap = d_fir_fff.ntaps();
            size_t htap = ntap>>1;
            size_t rtap = ntap-htap; //in case ntap is odd
            size_t nin = d_in_byte.size();
            size_t nbuf = nin + ntap;
            size_t nout = ((int)nin / d_decimation) + 1;
            if (d_ftmp.size() < nbuf)
            {
                d_ftmp.resize(nbuf);
            }
            memset(&d_ftmp[0], 0, sizeof(gr_complex)*htap);
            memset(&d_ftmp[nin+htap], 0, sizeof(gr_complex)*rtap);
            memcpy(&d_ftmp[0+htap], &d_in_byte[0], sizeof(gr_complex)*nin);
            d_out.resize(nout);
            // do FIR filtering
            d_fir_fff.filterNdec(d_out.data(), d_ftmp.data(), nout, d_decimation);
        }
        else
        {
            d_out.resize((d_in_byte.size() - d_fir_fff.ntaps() + 1) / d_decimation);

            // do FIR filtering
            d_fir_fff.filterNdec(d_out.data(), d_in.data() + d_pad, d_out.size(), d_decimation);
        }

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