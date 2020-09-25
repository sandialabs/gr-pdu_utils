/* -*- c++ -*- */
/*
 * Copyright 2018, 2019, 2020 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_PDU_UTILS_PDU_RANGE_FILTER_H
#define INCLUDED_PDU_UTILS_PDU_RANGE_FILTER_H

#include <gnuradio/block.h>
#include <pdu_utils/api.h>

namespace gr {
namespace pdu_utils {

/*!
 * \brief PDU filter K-V range
 * \ingroup pdu_utils
 *
 * Pass PDUs with a metadata value inside the given range. Block all others.
 *
 * Invert causes the block to pass packets it would normally block and block packets it
 * would normally pass.
 *
 */
class PDU_UTILS_API pdu_range_filter : virtual public gr::block
{
public:
    typedef boost::shared_ptr<pdu_range_filter> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of pdu_utils::pdu_range_filter.
     *
     * @param key - dict key to filter on
     * @param min - min value, inclusive
     * @param max - max value, inclusive
     * @param invert - false to pass values [min..max], true to pass outside range
     */
    static sptr make(pmt::pmt_t key, double min, double max, bool invert);
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_RANGE_FILTER_H */
