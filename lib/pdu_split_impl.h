/* -*- c++ -*- */
/*
 * Copyright 2018, 2019, 2020 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
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
