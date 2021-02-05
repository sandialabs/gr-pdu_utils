/* -*- c++ -*- */
/*
 * Copyright 2018-2021 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_PDU_UTILS_UPSAMPLE_IMPL_H
#define INCLUDED_PDU_UTILS_UPSAMPLE_IMPL_H

#include <pdu_utils/constants.h>
#include <pdu_utils/upsample.h>

namespace gr {
namespace pdu_utils {

class upsample_impl : public upsample
{
private:
    uint32_t d_n;
    bool d_repeat;

public:
    /**
     * Constructor
     *
     * @param n - upsample by factor n
     * @param repeat - true to repeat values, false to zero fill
     */
    upsample_impl(uint32_t n, bool repeat);

    /**
     * Deconstructor
     */
    ~upsample_impl();

    void handle_msg(pmt::pmt_t);

    /**
     * Set upsample factor
     *
     * @param n
     */
    void set_n(uint32_t n);

    /**
     * Set repeat behavior
     *
     * @param repeat = true to report values, false to zero fill
     */
    void set_repeat(bool repeat);
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_UPSAMPLE_IMPL_H */
