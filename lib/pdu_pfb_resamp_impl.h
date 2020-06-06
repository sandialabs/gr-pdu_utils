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

#ifndef INCLUDED_PDU_UTILS_PDU_PFB_RESAMP_IMPL_H
#define INCLUDED_PDU_UTILS_PDU_PFB_RESAMP_IMPL_H

#include <gnuradio/filter/pfb_arb_resampler.h>
#include <pdu_utils/constants.h>
#include <pdu_utils/pdu_pfb_resamp.h>
#include <boost/bind.hpp>
#include <boost/function.hpp>

namespace gr {
namespace pdu_utils {
template <class T, class S>
class pfb_filter_kernel
{
private:
    // resampling kernels
    gr::filter::kernel::pfb_arb_resampler_fff* d_pfb_fff;
    gr::filter::kernel::pfb_arb_resampler_ccf* d_pfb_ccf;
    gr::filter::kernel::pfb_arb_resampler_ccc* d_pfb_ccc;

public:
    pfb_filter_kernel(std::vector<S> ktaps, int n_filters);
    ~pfb_filter_kernel() {}

    // overloaded methods
    boost::function<int()> group_delay;
    boost::function<void(std::vector<S>)> set_taps;
    boost::function<std::vector<std::vector<S>>()> taps;
    boost::function<int(T*, T*, int, int&)> filter;
    boost::function<void(float)> set_rate;
};

template <class T, class S>
class pdu_pfb_resamp_impl : public pdu_pfb_resamp<T, S>
{
private:
    // overloaded initializer methods
    inline pmt::pmt_t init_data(float* data, int n)
    {
        return pmt::init_f32vector(n, data);
    }
    inline pmt::pmt_t init_data(gr_complex* data, int n)
    {
        return pmt::init_c32vector(n, data);
    }

private:
    pfb_filter_kernel<T, S>* d_pfb;
    T* d_in;
    T* d_out;
    size_t d_input_size;
    uint32_t d_nfilts;
    float d_resamp_rate;

    void resize_arrays(size_t newSize);

public:
    /**
     * Constructor
     *
     * @param taps -
     * @param n_filters -
     * @param resamp_rate -
     */
    pdu_pfb_resamp_impl(const std::vector<S> taps, int n_filters, float resamp_rate);

    /**
     * Deconstructor
     */
    ~pdu_pfb_resamp_impl();

    void handle_pdu(pmt::pmt_t pdu);

    /**
     * Set taps
     *
     * @param taps -
     */
    void set_taps(std::vector<S> taps) { d_pfb->set_taps(taps); }
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_PFB_RESAMP_IMPL_H */
