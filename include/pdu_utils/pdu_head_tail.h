/* -*- c++ -*- */
/*
 * Copyright 2018-2021 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_PDU_UTILS_PDU_HEAD_TAIL_H
#define INCLUDED_PDU_UTILS_PDU_HEAD_TAIL_H

#include <gnuradio/block.h>
#include <pdu_utils/api.h>

namespace gr {
namespace pdu_utils {

/*!
 * \brief <+description of block+>
 * \ingroup pdu_utils
 *
 * Block accumulates the start and end of bursts and emits PDUs suitable for bit
 * rastering.
 *
 *
 */
class PDU_UTILS_API pdu_head_tail : virtual public gr::block
{
public:
    typedef std::shared_ptr<pdu_head_tail> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of pdu_utils::pdu_head_tail.
     *
     * @param pdu_type -
     * @param length -
     * @param histsize -
     */
    static sptr make(uint32_t pdu_type, uint32_t length, uint32_t histsize);

    /**
     * Set Length
     *
     * @param length -
     */
    virtual void set_length(uint32_t length) = 0;

    /**
     * Set Histogram size
     *
     * @param histsize -
     */
    virtual void set_histsize(uint32_t histsize) = 0;

    /**
     * Set whether to space out bytes or not
     *
     * @param spacebytes - insert space between bytes?
     */
    virtual void set_space_bytes(bool spacebytes) = 0;
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_HEAD_TAIL_H */
