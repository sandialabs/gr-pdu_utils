/* -*- c++ -*- */
/*
 * Copyright 2018, 2019, 2020 National Technology & Engineering Solutions of Sandia, LLC
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

public:
    /**
     * Constructor
     *
     * @param k - Key to add
     * @param v - Value to add
     */
    pdu_set_m_impl(pmt::pmt_t k, pmt::pmt_t v);

    /**
     * Deconstructor
     */
    ~pdu_set_m_impl();
    void setup_rpc(); // enable controlport

    void handle_msg(pmt::pmt_t msg);
    void handle_ctrl_msg(pmt::pmt_t msg);

    /**
     * Get key value
     *
     * @return pmt::pmt_t
     */
    pmt::pmt_t key() { return d_k; }

    /**
     * Get Val value
     *
     * @return pmt::pmt_t
     */
    pmt::pmt_t val() { return d_v; }

    /**
     * Set Key Value
     *
     * @param k - key value
     */
    void set_key(pmt::pmt_t k);

    /**
     * Set Val Value
     *
     * @param v - value
     */
    void set_val(pmt::pmt_t v);

    /**
     * Set K-V pair
     *
     * @param k - key
     * @param v - val
     */
    void set_kv(pmt::pmt_t k, pmt::pmt_t v);
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_SET_M_IMPL_H */
