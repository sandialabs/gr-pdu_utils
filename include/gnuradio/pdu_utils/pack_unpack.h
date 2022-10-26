/* -*- c++ -*- */
/*
 * Copyright 2018-2021 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_PDU_UTILS_PACK_UNPACK_H
#define INCLUDED_PDU_UTILS_PACK_UNPACK_H

#include <gnuradio/block.h>
#include <gnuradio/pdu_utils/api.h>

namespace gr {
namespace pdu_utils {

/*!
 * \brief pack/unpack operations on uint8 PDUs
 * \ingroup pdu_utils
 *
 * This block operates on uint8 PDUs and can pack/unpack bits or swap the bit order of a
 * uint8 byte PDU.
 *
 *
 */
class PDU_UTILS_API pack_unpack : virtual public gr::block
{
public:
    typedef std::shared_ptr<pack_unpack> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of pdu_utils::pack_unpack.
     *
     * @param mode - operation mode, MODE_UNPACK_BYTE, MODE_PACK_BYTE, MODE_BITSWAP_BYTE
     * @param bitorder - BIT_ORDER_LSB_FIRST, BIT_ORDER_MSB_FIRST
     */
    static sptr make(uint32_t mode, uint32_t bitorder);

    /*
     * these are intentionally not exposed in GRC as they should not change
     * on the fly... for test only
     */

    /**
     * Sets Mode
     *
     * @param mode - MODE_UNPACK_BYTE, MODE_PACK_BYTE, MODE_BITSWAP_BYTE
     */
    virtual void set_mode(uint32_t mode) = 0;

    /**
     * Sets order mode
     *
     * @param order - BIT_ORDER_LSB_FIRST, BIT_ORDER_MSB_FIRST
     */
    virtual void set_bit_order(uint32_t order) = 0;
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PACK_UNPACK_H */
