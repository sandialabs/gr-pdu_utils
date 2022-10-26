/* -*- c++ -*- */
/*
 * Copyright 2018-2021 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_PDU_UTILS_PDU_LENGTH_FILTER_H
#define INCLUDED_PDU_UTILS_PDU_LENGTH_FILTER_H

#include <gnuradio/block.h>
#include <gnuradio/pdu_utils/api.h>

namespace gr {
namespace pdu_utils {

/*!
 * \brief PDU filtering based on length
 * \ingroup pdu_utils
 *
 * This block will selectively filter PDUs based on length of the data inside the PDU
 *
 */
class PDU_UTILS_API pdu_length_filter : virtual public gr::block
{
public:
    typedef std::shared_ptr<pdu_length_filter> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of pdu_utils::pdu_length_filter.
     *
     * @param length - length of data threshold
     * @param drop_long - true to drop PDUs with >length, false to drop PDUs with <length
     */
    static sptr make(uint32_t length, bool drop_long);
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_LENGTH_FILTER_H */
