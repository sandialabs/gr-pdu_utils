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

#ifndef INCLUDED_PDU_UTILS_SANDIA_MESSAGE_DEBUG_H
#define INCLUDED_PDU_UTILS_SANDIA_MESSAGE_DEBUG_H

#include <gnuradio/sync_block.h>
#include <pdu_utils/api.h>

namespace gr {
namespace pdu_utils {

/*!
 * \brief Provides STDOUT message logging capabilities
 * \ingroup pdu_utils
 *
 * Provides Message printing to STDOUT for debug purposes.
 *
 * 3 ports are provided, each with different functionality.
 * -print         simply renders the PMT using pmt::write_string() to STDOUT
 * -store         stores the PMT in an internal vector for later retrival
 * -print_pdu     expects the incoming message to be a PMT pair of meta data and data
 *
 * utilizes pmt::write_string for meta data portion
 * expects the data portion to be a u8vector, if not,
 * the data portion won't be rendered prints out data
 * portion as a hex dump
 */
class PDU_UTILS_API sandia_message_debug : virtual public gr::block
{
public:
    typedef boost::shared_ptr<sandia_message_debug> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of pdu_utils::sandia_message_debug.
     *
     *
     */
    static sptr make();

    /**
     * Returns the number of messages stored
     *
     * @return int - number of messages stored
     */
    virtual int num_messages() = 0;

    /**
     * Returns a specific stored message
     *
     * @param i - message index
     * @return pmt::pmt_t - stored message
     */
    virtual pmt::pmt_t get_message(int i) = 0;
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_SANDIA_MESSAGE_DEBUG_H */
