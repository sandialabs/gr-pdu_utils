/* -*- c++ -*- */
/*
 * Copyright 2018, 2019, 2020 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_PDU_UTILS_PDU_DOWNSAMPLE_IMPL_H
#define INCLUDED_PDU_UTILS_PDU_DOWNSAMPLE_IMPL_H

#include <pdu_utils/pdu_downsample.h>

namespace gr {
namespace pdu_utils {

class pdu_downsample_impl : public pdu_downsample
{
private:
    int d_decimation;
    int d_phase;

public:
    /**
     * Constructor
     *
     * @param decimation - decimation factor
     * @param phase - offset into each PDU
     */
    pdu_downsample_impl(int decimation, int phase);

    /**
     * Deconstructor
     */
    ~pdu_downsample_impl();

    void handle_msg(pmt::pmt_t pdu);

    // Where all the action really happens
    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_DOWNSAMPLE_IMPL_H */
