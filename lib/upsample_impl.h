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

#include <gnuradio/pdu_utils/constants.h>
#include <gnuradio/pdu_utils/upsample.h>

namespace gr {
namespace pdu_utils {

class upsample_impl : public upsample
{
private:
    uint32_t d_n;
    bool d_repeat;

    void handle_msg(pmt::pmt_t);

public:
    upsample_impl(uint32_t n, bool repeat);

    ~upsample_impl() override;

    void set_n(uint32_t n) override;
    void set_repeat(bool repeat) override;
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_UPSAMPLE_IMPL_H */
