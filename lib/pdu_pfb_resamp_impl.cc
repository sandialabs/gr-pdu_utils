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

#include "complex"
#include "pdu_pfb_resamp_impl.h"
#include <gnuradio/io_signature.h>
#include <volk/volk.h>

namespace gr {
namespace pdu_utils {

template <class T, class S>
typename pdu_pfb_resamp<T, S>::sptr
pdu_pfb_resamp<T, S>::make(const std::vector<S> taps, int n_filters, float resamp_rate)
{
    return gnuradio::get_initial_sptr(
        new pdu_pfb_resamp_impl<T, S>(taps, n_filters, resamp_rate));
}

/*
 * The private constructor
 */
template <class T, class S>
pdu_pfb_resamp_impl<T, S>::pdu_pfb_resamp_impl(const std::vector<S> taps,
                                               int n_filters,
                                               float resamp_rate)
    : gr::block("pdu_pfb_resamp",
                gr::io_signature::make(0, 0, 0),
                gr::io_signature::make(0, 0, 0)),
      d_nfilts(n_filters),
      d_resamp_rate(resamp_rate)
{
    d_pfb = new pfb_filter_kernel<T, S>(taps, n_filters);
    d_pfb->set_rate(d_resamp_rate);

    d_in = NULL;
    d_out = NULL;
    d_input_size = 0;

    // GR_LOG_DEBUG(this->d_logger,
    //   boost::format("started pdu pfb resampler with %d taps") %
    //   int(d_taps.size()));

    this->message_port_register_in(PMTCONSTSTR__pdu_in());
    this->set_msg_handler(PMTCONSTSTR__pdu_in(),
                          boost::bind(&pdu_pfb_resamp_impl<T, S>::handle_pdu, this, _1));
    this->message_port_register_out(PMTCONSTSTR__pdu_out());
}

/*group_delay
 * Our virtual destructor.
 */
template <class T, class S>
pdu_pfb_resamp_impl<T, S>::~pdu_pfb_resamp_impl()
{
    if (d_in != NULL)
        volk_free(d_in);
    if (d_out != NULL)
        volk_free(d_out);
}

template <class T, class S>
void pdu_pfb_resamp_impl<T, S>::resize_arrays(size_t newSize)
{
    if (d_in != NULL)
        volk_free(d_in);
    if (d_out != NULL)
        volk_free(d_out);
    d_in = (T*)volk_malloc(sizeof(T) * newSize, volk_get_alignment());
    d_out =
        (T*)volk_malloc((sizeof(T) * newSize * d_resamp_rate + 1), volk_get_alignment());
    d_input_size = newSize;
}

template <class T, class S>
void pdu_pfb_resamp_impl<T, S>::handle_pdu(pmt::pmt_t pdu)
{

    /* code */
    pmt::pmt_t meta = pmt::car(pdu);
    pmt::pmt_t v_data = pmt::cdr(pdu);
    if (pmt::is_uniform_vector(v_data)) {
        size_t nbytes = 0;
        size_t nitems = 0;
        const T* input_data =
            static_cast<const T*>(pmt::uniform_vector_elements(v_data, nbytes));
        if (nbytes % sizeof(T) != 0) {
            GR_LOG_WARN(this->d_logger,
                        "Potentially conflicting PDU data type...aborting");
            return;
        }
        nitems = nbytes / sizeof(T);

        // allocate enough space to pad with 1/2 filter length on each side of the data
        // (group delay) also tack on some extra memory because the in-tree filter() call
        // accesses out-of-bounds memory
        if (d_input_size < nitems + d_nfilts * 2) {
            // Give ourselves some extra size to prevent a ton of reallocs
            resize_arrays(nitems * 1.25 + d_nfilts * 2);
        }
        const float z = 0.0;
        int start = d_nfilts / 2;
        int num_read;

        // adjust length by filter taps
        // we'll also add on extra memory space at the end to protect against how
        // gr::filter works (out of bounds memory access)
        for (int ii = 0; ii < start; ii++)
            d_in[ii] = input_data[0];
        memcpy(&d_in[start], &input_data[0], sizeof(T) * nitems);
        for (int ii = 0; ii < 2 * start; ii++)
            d_in[ii + start + nitems] = input_data[nitems - 1];
        // for (int ii=0; ii < start; ii++) d_in[ii+start+nitems] = input_data[nitems-1];
        // // this is if filter() is ever fixed

        int n_out = d_pfb->filter(d_out, d_in, nitems + start, num_read);

        if (pmt::dict_has_key(meta, PMTCONSTSTR__sample_rate())) {
            double sample_rate =
                pmt::to_double(pmt::dict_ref(meta, PMTCONSTSTR__sample_rate(), pmt::PMT_NIL));
            sample_rate *= d_resamp_rate;
            meta = pmt::dict_delete(meta, PMTCONSTSTR__sample_rate());
            meta = pmt::dict_add(
                meta, PMTCONSTSTR__sample_rate(), pmt::from_double(sample_rate));
        }

        // publish message
        this->message_port_pub(PMTCONSTSTR__pdu_out(),
                               pmt::cons(meta, this->init_data(d_out, n_out)));

    } else {
        GR_LOG_WARN(this->d_logger, "Received an invalid PDU");
    }
}

/* ===========================================================================
 * Filter kernels
 ===========================================================================*/
template <class T, class S>
pfb_filter_kernel<T, S>::pfb_filter_kernel(std::vector<S> taps, int n_filters)
{
    throw std::runtime_error("Invalid PDU PFB filter kernel type");
}

template <>
pfb_filter_kernel<float, float>::pfb_filter_kernel(std::vector<float> ktaps,
                                                   int n_filters)
{
    // instantiate correct kernel
    d_pfb_fff = new gr::filter::kernel::pfb_arb_resampler_fff(1, ktaps, n_filters);

    // bind methods
    group_delay =
        boost::bind(&gr::filter::kernel::pfb_arb_resampler_fff::group_delay, d_pfb_fff);
    set_taps =
        boost::bind(&gr::filter::kernel::pfb_arb_resampler_fff::set_taps, d_pfb_fff, _1);
    taps = boost::bind(&gr::filter::kernel::pfb_arb_resampler_fff::taps, d_pfb_fff);
    set_rate =
        boost::bind(&gr::filter::kernel::pfb_arb_resampler_fff::set_rate, d_pfb_fff, _1);
    filter = boost::bind(
        &gr::filter::kernel::pfb_arb_resampler_fff::filter, d_pfb_fff, _1, _2, _3, _4);
}

template <>
pfb_filter_kernel<gr_complex, float>::pfb_filter_kernel(std::vector<float> ktaps,
                                                        int n_filters)
{
    // instantiate correct kernel
    d_pfb_ccf = new gr::filter::kernel::pfb_arb_resampler_ccf(1, ktaps, n_filters);
    // bind methods
    group_delay =
        boost::bind(&gr::filter::kernel::pfb_arb_resampler_ccf::group_delay, d_pfb_ccf);
    set_taps =
        boost::bind(&gr::filter::kernel::pfb_arb_resampler_ccf::set_taps, d_pfb_ccf, _1);
    taps = boost::bind(&gr::filter::kernel::pfb_arb_resampler_ccf::taps, d_pfb_ccf);
    set_rate =
        boost::bind(&gr::filter::kernel::pfb_arb_resampler_ccf::set_rate, d_pfb_ccf, _1);
    filter = boost::bind(
        &gr::filter::kernel::pfb_arb_resampler_ccf::filter, d_pfb_ccf, _1, _2, _3, _4);
}

template <>
pfb_filter_kernel<gr_complex, gr_complex>::pfb_filter_kernel(
    std::vector<gr_complex> ktaps, int n_filters)
{
    // instantiate correct kernel
    d_pfb_ccc = new gr::filter::kernel::pfb_arb_resampler_ccc(1, ktaps, n_filters);

    // bind methods
    group_delay =
        boost::bind(&gr::filter::kernel::pfb_arb_resampler_ccc::group_delay, d_pfb_ccc);
    set_taps =
        boost::bind(&gr::filter::kernel::pfb_arb_resampler_ccc::set_taps, d_pfb_ccc, _1);
    taps = boost::bind(&gr::filter::kernel::pfb_arb_resampler_ccc::taps, d_pfb_ccc);
    set_rate =
        boost::bind(&gr::filter::kernel::pfb_arb_resampler_ccc::set_rate, d_pfb_ccc, _1);
    filter = boost::bind(
        &gr::filter::kernel::pfb_arb_resampler_ccc::filter, d_pfb_ccc, _1, _2, _3, _4);
}

// only support float and gr_complex
template class pdu_pfb_resamp<float, float>;
template class pdu_pfb_resamp<gr_complex, float>;
template class pdu_pfb_resamp<gr_complex, gr_complex>;
} /* namespace pdu_utils */
} /* namespace gr */
