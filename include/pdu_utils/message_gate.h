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


#ifndef INCLUDED_PDU_UTILS_MESSAGE_GATE_H
#define INCLUDED_PDU_UTILS_MESSAGE_GATE_H

#include <gnuradio/block.h>
#include <pdu_utils/api.h>

namespace gr {
namespace pdu_utils {

/*!
 * \brief Simple message gate
 * \ingroup pdu_utils
 *
 * Simple message gate to control whether or not messages pass.
 *
 */
class PDU_UTILS_API message_gate : virtual public gr::block
{
public:
    typedef boost::shared_ptr<message_gate> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of pdu_utils::message_gate.
     *
     * @param enabled - true to pass messages
     */
    static sptr make(bool enabled);

    /**
     * Return count of passed messages
     *
     * @return uint64_t
     */
    virtual uint64_t get_n_passed(void) = 0;

    /**
     * Return count of blocked messages
     *
     * @return uint64_t
     */
    virtual uint64_t get_n_blocked(void) = 0;

    /**
     * Returns enable status
     *
     * @return bool - true when messages passed
     */
    virtual bool get_enabled(void) = 0;

    /**
     * Set gate state
     *
     * @param enabled - true to pass messages
     */
    virtual void set_enabled(bool enabled) = 0;
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_MESSAGE_GATE_H */
