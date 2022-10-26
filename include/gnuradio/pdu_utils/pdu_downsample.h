/* -*- c++ -*- */
/*
 * Copyright 2018-2021 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_PDU_UTILS_PDU_DOWNSAMPLE_H
#define INCLUDED_PDU_UTILS_PDU_DOWNSAMPLE_H

#include <gnuradio/block.h>
#include <gnuradio/pdu_utils/api.h>

namespace gr {
namespace pdu_utils {

/*!
 * \brief <+description of block+>
 * \ingroup pdu_utils
 *
 * Downsample the vector poriton of a pdu by decimation starting at index phase.
 *
 * Can handle floats or uint8s
 *
 */
class PDU_UTILS_API pdu_downsample : virtual public gr::block
{
public:
    typedef std::shared_ptr<pdu_downsample> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of pdu_utils::pdu_downsample.
     *
     * @param decimation - decimation factor
     * @param phase - offset into each PDU
     */
    static sptr make(int decimation, int phase);
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_DOWNSAMPLE_H */
