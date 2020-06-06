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

#ifndef INCLUDED_PDU_UTILS_PDU_SPLIT_IMPL_H
#define INCLUDED_PDU_UTILS_PDU_SPLIT_IMPL_H

#include <pdu_utils/constants.h>
#include <pdu_utils/pdu_split.h>

namespace gr {
namespace pdu_utils {

class pdu_split_impl : public pdu_split
{
private:
    bool d_pass_empty_data;

public:
    /**
     * Constructor
     *
     * @param pass_empty_data - true to pass empty vectors
     */
    pdu_split_impl(bool pass_empty_data);
    /**
     * Deconstructor
     */
    ~pdu_split_impl();

    void handle_pdu(pmt::pmt_t pdu);
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_SPLIT_IMPL_H */
