/* -*- c++ -*- */
/*
 * Copyright 2018-2021 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_PDU_UTILS_PDU_FLOW_CTRL_HELPER_H
#define INCLUDED_PDU_UTILS_PDU_FLOW_CTRL_HELPER_H

#include <gnuradio/basic_block.h>
#include <gnuradio/pdu_utils/api.h>

namespace gr {
namespace pdu_utils {

typedef std::weak_ptr<gr::basic_block> basic_block_wptr;

/*!
 * \brief Convenience functions for watching the number of messages waiting
 * at the given blocks' inputs.
 */
class PDU_UTILS_API pdu_flow_ctrl_helper
{
public:
    /**
     * Constructor
     *
     * @param blocks -
     */
    pdu_flow_ctrl_helper(std::vector<gr::basic_block_sptr> blocks);

    /**
     * Deconstructor
     */
    virtual ~pdu_flow_ctrl_helper();

    int max_nmsgs();
    void print_nmsgs();

private:
    struct block_port {
        basic_block_wptr block;
        std::vector<pmt::pmt_t> ports;
    };

    std::vector<block_port> d_blocks;
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_FLOW_CTRL_HELPER_H */
