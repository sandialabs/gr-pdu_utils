/* -*- c++ -*- */
/*
 * Copyright 2018-2021 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_PDU_UTILS_MESSAGE_KEEP_1_IN_N_IMPL_H
#define INCLUDED_PDU_UTILS_MESSAGE_KEEP_1_IN_N_IMPL_H

#include <gnuradio/pdu_utils/constants.h>
#include <gnuradio/pdu_utils/message_keep_1_in_n.h>

namespace gr {
namespace pdu_utils {

class message_keep_1_in_n_impl : public message_keep_1_in_n
{
private:
    uint32_t d_n;
    uint32_t d_ctr;

    /*!
     * \brief Message handler for input messages
     *
     * \param msg Any PMT message passed from the scheduler's message handling.
     */
    void handle_msg(pmt::pmt_t msg);

public:
    message_keep_1_in_n_impl(uint32_t n);

    ~message_keep_1_in_n_impl() override;

    void set_n(uint32_t n) override;
    uint32_t get_n(void) override { return d_n; }
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_MESSAGE_KEEP_1_IN_N_IMPL_H */
