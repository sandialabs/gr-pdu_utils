/* -*- c++ -*- */
/*
 * Copyright 2018-2021 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_PDU_UTILS_PDU_SET_M_H
#define INCLUDED_PDU_UTILS_PDU_SET_M_H

#include <gnuradio/block.h>
#include <pdu_utils/api.h>

namespace gr {
namespace pdu_utils {

/*!
 * \brief Set K-V on PDUs
 * \ingroup pdu_utils
 *
 * This block is identical to the in-tree PDU Set block except it
 * has callbacks for dynamically updating the KV pair.
 *
 * This minor change should be PR'd in tree.
 */
class PDU_UTILS_API pdu_set_m : virtual public gr::block
{
public:
    typedef std::shared_ptr<pdu_set_m> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of pdu_utils::pdu_set_m.
     *
     * @param k - Key to add
     * @param v - Value to add
     */
    static sptr make(pmt::pmt_t k, pmt::pmt_t v);

    /**
     * Get key value
     *
     * @return pmt::pmt_t
     */
    virtual pmt::pmt_t key(void) = 0;

    /**
     * Get Val value
     *
     * @return pmt::pmt_t
     */
    virtual pmt::pmt_t val(void) = 0;

    /**
     * Set Key Value
     *
     * @param k - key value
     */
    virtual void set_key(pmt::pmt_t k) = 0;

    /**
     * Set Val Value
     *
     * @param v - value
     */
    virtual void set_val(pmt::pmt_t v) = 0;

    /**
     * Set K-V pair
     *
     * @param k - key
     * @param v - val
     */
    virtual void set_kv(pmt::pmt_t k, pmt::pmt_t v) = 0;
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_SET_M_H */
