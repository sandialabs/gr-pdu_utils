/* -*- c++ -*- */
/*
 * Copyright 2018-2021 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_PDU_UTILS_PDU_SPLIT_H
#define INCLUDED_PDU_UTILS_PDU_SPLIT_H

#include <gnuradio/block.h>
#include <pdu_utils/api.h>

namespace gr {
namespace pdu_utils {

/*!
 * \brief Split dict and data
 * \ingroup pdu_utils
 *
 * Splits a PDU into its metadata dictionary and vector,
 * outputs nothing if not a PDU. Useful for stripping/viewing metadata.
 *
 *
 */
class PDU_UTILS_API pdu_split : virtual public gr::block
{
public:
    typedef std::shared_ptr<pdu_split> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of pdu_utils::pdu_split.
     *
     * @param pass_empty_data - true to pass empty vectors
     */
    static sptr make(bool pass_empty_data = false);
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_SPLIT_H */
