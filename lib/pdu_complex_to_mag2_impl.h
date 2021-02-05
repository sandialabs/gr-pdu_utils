/* -*- c++ -*- */
/*
 * Copyright 2018-2021 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_PDU_UTILS_PDU_COMPLEX_TO_MAG2_IMPL_H
#define INCLUDED_PDU_UTILS_PDU_COMPLEX_TO_MAG2_IMPL_H

#include <pdu_utils/pdu_complex_to_mag2.h>

namespace gr {
namespace pdu_utils {

class pdu_complex_to_mag2_impl : public pdu_complex_to_mag2
{
private:
    // Nothing to declare in this block.

public:
    /**
     * Constructor
     */
    pdu_complex_to_mag2_impl();

    /**
     * Deconstructor
     */
    ~pdu_complex_to_mag2_impl();

    // Where all the action really happens
    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);

    void handle_pdu(pmt::pmt_t pdu);
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_COMPLEX_TO_MAG2_IMPL_H */
