/* -*- c++ -*- */
/*
 * Copyright 2018-2021 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_PDU_UTILS_PDU_BURST_COMBINER_H
#define INCLUDED_PDU_UTILS_PDU_BURST_COMBINER_H

#include <gnuradio/block.h>
#include <gnuradio/pdu_utils/api.h>

namespace gr {
namespace pdu_utils {

/*!
 * \brief c32 PDU combiner
 * \ingroup pdu_utils
 *
 * Combines multiple bursts into a single burst
 *
 * Only operates on PDUs with c32vector data
 *
 * Uses burst_index meta data
 */
class PDU_UTILS_API pdu_burst_combiner : virtual public gr::block
{
public:
    typedef std::shared_ptr<pdu_burst_combiner> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of pdu_utils::pdu_burst_combiner.
     *
     * To avoid accidental use of raw pointers, pdu_utils::pdu_burst_combiner's
     * constructor is in a private implementation
     * class. pdu_utils::pdu_burst_combiner::make is the public interface for
     * creating new instances.
     */
    static sptr make();
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_BURST_COMBINER_H */
