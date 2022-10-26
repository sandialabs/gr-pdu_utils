/* -*- c++ -*- */
/*
 * Copyright 2018, 2019, 2020 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_PDU_UTILS_PDU_SET_M_H
#define INCLUDED_PDU_UTILS_PDU_SET_M_H

#include <gnuradio/block.h>
#include <gnuradio/pdu_utils/api.h>

namespace gr {
namespace pdu_utils {

/*!
 * \brief Set K-V on PDUs
 * \ingroup pdu_utils
 *
 * This block is an augmented version of the PDU Set block. The key and value
 * can be dynamically updated.
 *
 * If the kv_merge boolean is set to true, the value is parsed as a string; if
 * there is a string between curly braces, the block searches through the PDU
 * dictionary for a key matching that string. If the key is found, the string
 * is replaced with the value from the PDU. For example, if the key/value pair
 * in the block is 'serial: SN-{serialnumber}', and the incoming PDU has a
 * key/value pair of 'serialnumber: 123', the output PDU will have two key/value
 * pairs: 'serialnumber: 123' and 'serial: SN-123'.
 *
 * If the v_overwrite boolean is set to false, then the PDU will not update the
 * key/value pair if the key is already present in the PDU.
 *
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
     * @param kv_merge - If true, parse v for keys and replace it with any
     *          corresponding values in the incoming metadata
     * @param v_overwrite - If true, if k is already found in the metadata,
     *          overwrite the value with desired value; otherwise skip
     */
    static sptr make(pmt::pmt_t k, pmt::pmt_t v, bool kv_merge=false, bool v_overwrite=false);

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
     * Set Parsed Val
     *
     * @param dict - metadata of incoming PDU
     */
    virtual pmt::pmt_t parse_val(pmt::pmt_t dict) = 0;

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
     * Set Merge Value
     *
     * @param kv_merge - merge boolean
     */
    virtual void set_kv_merge(bool kv_merge) = 0;

    /**
     * Set Overwrite Value
     *
     * @param v_overwrite - overwrite boolean
     */
    virtual void set_v_overwrite(bool v_overwrite) = 0;

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
