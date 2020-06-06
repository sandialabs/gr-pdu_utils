/* -*- c++ -*- */
/*
 * Copyright 2018 National Technology & Engineering Solutions of Sandia, LLC (NTESS).
 * Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government retains
 * certain rights in this software.
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
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
    typedef boost::shared_ptr<pdu_head_tail> sptr;

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
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_HEAD_TAIL_H */
