/* -*- c++ -*- */
/*
 * Copyright 2018-2021 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_PDU_UTILS_PDU_BINARY_TOOLS_IMPL_H
#define INCLUDED_PDU_UTILS_PDU_BINARY_TOOLS_IMPL_H

#include <gnuradio/pdu_utils/pdu_binary_tools.h>

namespace gr {
namespace pdu_utils {

class pdu_binary_tools_impl : public pdu_binary_tools
{
private:
    void handle_msg_bit_flip(pmt::pmt_t pdu);
    void handle_msg_to_nrz(pmt::pmt_t pdu);
    void handle_msg_from_nrz(pmt::pmt_t pdu);
    void handle_msg_slice(pmt::pmt_t pdu);
    void handle_msg_endian8(pmt::pmt_t pdu);
    void handle_msg_passthrough(pmt::pmt_t pdu);
    void handle_msg_manchester_encode(pmt::pmt_t pdu);
    void handle_msg_manchester_decode(pmt::pmt_t pdu);


public:
    pdu_binary_tools_impl(uint8_t mode);

    ~pdu_binary_tools_impl();


};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_BINARY_TOOLS_IMPL_H */
