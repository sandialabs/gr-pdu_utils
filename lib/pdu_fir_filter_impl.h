/* -*- c++ -*- */
/*
 * Copyright 2018-2021 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_PDU_UTILS_PDU_FIR_FILTER_IMPL_H
#define INCLUDED_PDU_UTILS_PDU_FIR_FILTER_IMPL_H

#include <gnuradio/filter/fir_filter.h>
#include <pdu_utils/constants.h>
#include <pdu_utils/pdu_fir_filter.h>

namespace gr {
namespace pdu_utils {

class pdu_fir_filter_impl : public pdu_fir_filter
{
private:
    filter::kernel::fir_filter_fff d_fir_fff;
    filter::kernel::fir_filter_ccf d_fir_ccf;
    int d_decimation;
    std::vector<gr_complex> d_tmp;
    size_t d_pad;
    size_t d_group_delay_offset;
    bool d_even_num_taps;
    gr::thread::mutex d_mutex;


public:
    /**
     * Constructor
     *
     * @param decimation - decimation factor to apply
     * @param taps - FIR taps
     */
    pdu_fir_filter_impl(int decimation, const std::vector<float> taps);

    /**
     * Deconstructor
     */
    ~pdu_fir_filter_impl();

    void handle_pdu(pmt::pmt_t pdu);

    /**
     * Set FIR taps
     *
     * @param taps - FIR taps
     */
    void set_taps(std::vector<float> taps);
    /**
     * Set Decimation factor
     *
     * @param decimation - decimation factor
     */
    void set_decimation(int decimation) { d_decimation = decimation; }
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_FIR_FILTER_IMPL_H */
