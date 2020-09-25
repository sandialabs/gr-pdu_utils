/* -*- c++ -*- */
/*
 * Copyright 2018, 2019, 2020 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_PDU_UTILS_PDU_RANGE_FILTER_IMPL_H
#define INCLUDED_PDU_UTILS_PDU_RANGE_FILTER_IMPL_H

#include <pdu_utils/constants.h>
#include <pdu_utils/pdu_range_filter.h>

namespace gr {
namespace pdu_utils {

class pdu_range_filter_impl : public pdu_range_filter
{
private:
    pmt::pmt_t d_key;
    double d_min;
    double d_max;
    bool d_invert;

    void pdu_handler(pmt::pmt_t pdu);

public:
    /**
     * Constructor
     *
     * @param key - dict key to filter on
     * @param min - min value, inclusive
     * @param max - max value, inclusive
     * @param invert - false to pass values [min..max], true to pass outside range
     */
    pdu_range_filter_impl(pmt::pmt_t key, double min, double max, bool invert);

    /**
     * Deconstructor
     */
    ~pdu_range_filter_impl();

    void set_key(pmt::pmt_t key) { d_key = key; }
    void set_min(double min) { d_min = min; }
    void set_max(double max) { d_max = max; }
    void set_inversion(bool invert) { d_invert = invert; }
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_RANGE_FILTER_IMPL_H */
