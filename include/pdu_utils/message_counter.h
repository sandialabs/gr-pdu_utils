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


#ifndef INCLUDED_PDU_UTILS_MESSAGE_COUNTER_H
#define INCLUDED_PDU_UTILS_MESSAGE_COUNTER_H

#include <gnuradio/block.h>
#include <pdu_utils/api.h>

namespace gr {
namespace pdu_utils {

/*!
 * \brief Counts the number of message received by the msg port
 *
 * Prints out message count when the flow graph stops
 *
 * \ingroup pdu_utils
 *
 */
class PDU_UTILS_API message_counter : virtual public gr::block
{
public:
    typedef boost::shared_ptr<message_counter> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of pdu_utils::message_counter.
     *
     * To avoid accidental use of raw pointers, pdu_utils::message_counter's
     * constructor is in a private implementation
     * class. pdu_utils::message_counter::make is the public interface for
     * creating new instances.
     *
     * @param name - name of counter
     */
    static sptr make(pmt::pmt_t name);

    /**
     * resets the counter to zero
     */
    virtual void reset(void) = 0;

    /**
     * returns the current value of the counter
     *
     * @return uint64_t - counter value.
     */
    virtual uint64_t get_ctr(void) = 0;
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_MESSAGE_COUNTER_H */
