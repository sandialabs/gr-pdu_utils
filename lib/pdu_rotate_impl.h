/* -*- c++ -*- */
/*
 * Copyright 2020-2021 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_PDU_UTILS_PDU_ROTATE_IMPL_H
#define INCLUDED_PDU_UTILS_PDU_ROTATE_IMPL_H

#include <gnuradio/blocks/rotator.h>
#include <pdu_utils/pdu_rotate.h>

namespace gr {
namespace pdu_utils {

class pdu_rotate_impl : public pdu_rotate
{
private:
    blocks::rotator d_r;
    double d_phase_inc;
    void pdu_handler(pmt::pmt_t pdu);

public:
    pdu_rotate_impl(double phase_inc);

    ~pdu_rotate_impl() override;

    void set_phase_inc(double phase_inc) override;
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_ROTATE_IMPL_H */
