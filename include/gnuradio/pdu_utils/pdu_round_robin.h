/* -*- c++ -*- */
/*
 * Copyright 2018-2021 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_PDU_UTILS_PDU_ROUND_ROBIN_H
#define INCLUDED_PDU_UTILS_PDU_ROUND_ROBIN_H

#include <gnuradio/block.h>
#include <gnuradio/pdu_utils/api.h>

namespace gr {
namespace pdu_utils {

/*!
 * \brief Round Robin PDU delivery
 * \ingroup pdu_utils
 *
 * Round Robin delivers messages to a number of output paths.
 * In other Words Load shares messages across a number of outputs
 */
class PDU_UTILS_API pdu_round_robin : virtual public gr::block
{
public:
    typedef std::shared_ptr<pdu_round_robin> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of pdu_utils::pdu_round_robin.
     *
     * @param num_outputs - number of output ports
     */
    static sptr make(int num_outputs);
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_ROUND_ROBIN_H */
