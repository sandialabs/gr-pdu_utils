/* -*- c++ -*- */
/*
 * Copyright 2018-2021 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_PDU_UTILS_PDU_SET_M_IMPL_H
#define INCLUDED_PDU_UTILS_PDU_SET_M_IMPL_H

#include <pdu_utils/constants.h>
#include <pdu_utils/pdu_set_m.h>

namespace gr {
namespace pdu_utils {

class pdu_set_m_impl : public pdu_set_m
{
private:
    pmt::pmt_t d_k;
    pmt::pmt_t d_v;

    void handle_msg(pmt::pmt_t msg);
    void handle_ctrl_msg(pmt::pmt_t msg);

public:
    pdu_set_m_impl(pmt::pmt_t k, pmt::pmt_t v);

    ~pdu_set_m_impl() override;
    void setup_rpc() override; // enable controlport

    pmt::pmt_t key() override { return d_k; }
    pmt::pmt_t val() override { return d_v; }
    void set_key(pmt::pmt_t k) override;
    void set_val(pmt::pmt_t v) override;
    void set_kv(pmt::pmt_t k, pmt::pmt_t v) override;
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_SET_M_IMPL_H */
