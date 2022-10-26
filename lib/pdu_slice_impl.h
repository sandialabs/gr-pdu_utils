/* -*- c++ -*- */
/*
 * Copyright 2018, 2019, 2020 National Technology & Engineering Solutions of Sa>
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Gover>
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_PDU_UTILS_PDU_SLICE_IMPL_H
#define INCLUDED_PDU_UTILS_PDU_SLICE_IMPL_H

#include "gnuradio/pdu_utils/constants.h"
#include <gnuradio/io_signature.h>
#include <gnuradio/pdu_utils/pdu_slice.h>

namespace gr {
namespace pdu_utils {
class pdu_slice_impl : public pdu_slice
{
private:
    template<typename T>
    void slice_vector(std::vector<T> &in_data, std::vector<T> &out_data);

private:
    std::string d_slice;
    int d_start_index;
    int d_stop_index;
    int d_stride_size;
    bool d_blank_start;
    bool d_blank_stop;

public:
    /**
     * Constructor
     *
     * @param slice
     */
    pdu_slice_impl(std::string slice);

    /**
     * Deconstructor
     */
    ~pdu_slice_impl();

    void handle_pdu(pmt::pmt_t pdu);

    /**
     * Sets slice notation
     *
     * @param slice
     */
    void set_slice(std::string slice);
    

};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_SLICE_IMPL_H */
