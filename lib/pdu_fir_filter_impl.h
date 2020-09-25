/* -*- c++ -*- */
/*
 * Copyright 2018, 2019, 2020 National Technology & Engineering Solutions of Sandia, LLC
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
    size_t 		d_pad;
    bool d_pad_for_filter;
    std::vector<gr_complex> d_ctmp;
    std::vector<float> d_ftmp;
public:
    /**
     * Constructor
     *
     * @param decimation - decimation factor to apply
     * @param taps - FIR taps
     * @param pad_for_filter - option to avoid filter tap loss in size
     */
    pdu_fir_filter_impl(int decimation, const std::vector<float> taps,
                        bool pad_for_filter=false);

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
    void set_taps(std::vector<float> taps)
    {
        d_fir_fff.set_taps(taps);
        d_fir_ccf.set_taps(taps);
    }

    /**
     * Set Decimation factor
     *
     * @param decimation - decimation factor
     */
    void set_decimation(int decimation) { d_decimation = decimation; }

    /**
     * Set Pad for Filter
     *
     * @param pad_for_filter - option to avoid filter tap loss in size
     */
    void set_pad_for_filter(bool pad_for_filter) { d_pad_for_filter = pad_for_filter; }
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_FIR_FILTER_IMPL_H */