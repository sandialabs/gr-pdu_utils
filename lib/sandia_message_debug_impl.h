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

#ifndef INCLUDED_PDU_UTILS_SANDIA_MESSAGE_DEBUG_IMPL_H
#define INCLUDED_PDU_UTILS_SANDIA_MESSAGE_DEBUG_IMPL_H

#include <pdu_utils/sandia_message_debug.h>

namespace gr {
namespace pdu_utils {

class sandia_message_debug_impl : public sandia_message_debug
{
private:
    /**
     * prints msg to STDOUT using pmt::write_string()
     *
     * @param msg - message to print
     */
    void print(pmt::pmt_t msg);

    /**
     * Prints a PMT message to STDOUT
     * Assumes incoming pmt is a PMT pair of meta data and u8vector data
     *
     * @param pdu - incoming PDU to print
     */
    void print_pdu(pmt::pmt_t pdu);

    /**
     * Stores msg in internal vector for later retrival
     *
     * @param msg - message to store
     */
    void store(pmt::pmt_t msg);

    gr::thread::mutex d_mutex;
    std::vector<pmt::pmt_t> d_messages;

public:
    /**
     * Constructor
     */
    sandia_message_debug_impl();

    /**
     * deconstructor
     */
    ~sandia_message_debug_impl();

    /**
     * Returns the number of messages stored
     *
     * @return int - number of messages stored
     */
    int num_messages();

    /**
     * Returns a specific stored message
     *
     * @param i - message index
     * @return pmt::pmt_t - stored message
     */
    pmt::pmt_t get_message(int i);
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_SANDIA_MESSAGE_DEBUG_IMPL_H */
