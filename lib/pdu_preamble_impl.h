/* -*- c++ -*- */
/*
 * Copyright 2018-2021 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_PDU_UTILS_PDU_PREAMBLE_IMPL_H
#define INCLUDED_PDU_UTILS_PDU_PREAMBLE_IMPL_H

#include <pdu_utils/constants.h>
#include <pdu_utils/pdu_preamble.h>

namespace gr {
namespace pdu_utils {

class pdu_preamble_impl : public pdu_preamble
{
private:
    std::vector<uint8_t> d_preamble;
    std::vector<float> d_preamble_interp;
    std::vector<uint8_t> d_tail;
    std::vector<float> d_tail_interp;
    std::vector<uint8_t> d_zeros;
    uint32_t d_interp;
    uint32_t d_zero_pad;
    bool d_nrz;

    void handle_msg(pmt::pmt_t pdu);

public:
    pdu_preamble_impl(const std::vector<uint8_t> preamble,
                      const std::vector<uint8_t> tail,
                      uint32_t interp = 1,
                      uint32_t zero_pad = 0,
                      bool nrz = true);

    ~pdu_preamble_impl() override;

    void set_preamble(const std::vector<uint8_t> preamble) override;
    void set_tail(const std::vector<uint8_t> tail) override;
    void set_interp(uint32_t interp) override;
    void set_zero_pad(uint32_t zero_pad) override;
    void set_nrz(bool nrz) override;
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_PREAMBLE_IMPL_H */
