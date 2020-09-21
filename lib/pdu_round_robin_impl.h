/* -*- c++ -*- */
/*
 * Copyright 2018, 2019, 2020 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_PDU_UTILS_PDU_ROUND_ROBIN_IMPL_H
#define INCLUDED_PDU_UTILS_PDU_ROUND_ROBIN_IMPL_H

#include <pdu_utils/pdu_round_robin.h>

namespace gr {
namespace pdu_utils {

class pdu_round_robin_impl : public pdu_round_robin
{
private:
    int d_num_outputs;
    int d_counter;

    std::vector<pmt::pmt_t> d_output_ports;

    void pdu_handler(pmt::pmt_t pdu);

public:
    /**
     * Constructor
     *
     * @param num_outputs - number of output ports
     */
    pdu_round_robin_impl(int num_outputs);

    /**
     * Deconstructor
     */
    ~pdu_round_robin_impl();
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_ROUND_ROBIN_IMPL_H */
