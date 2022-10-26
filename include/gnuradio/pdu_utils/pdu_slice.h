/* -*- c++ -*- */
/*
 * Copyright 2018, 2019, 2020 National Technology & Engineering Solutions of Sa>
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Gover>
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_PDU_UTILS_PDU_SLICE_H
#define INCLUDED_PDU_UTILS_PDU_SLICE_H

#include <gnuradio/block.h>
#include <gnuradio/pdu_utils/api.h>

namespace gr {
namespace pdu_utils {

/*!
 * \brief PDU Slice
 * \ingroup pdu_utils
 *
 * This block receives a PDU and outputs a sliced version of it, given
 * a start and stop index, and a stride size. The input notation and function
 * is identical to the Python slice notation. See Python documentation
 * on numpy slice notation for more information.
 */
class PDU_UTILS_API pdu_slice : virtual public gr::block
{
public:
    typedef std::shared_ptr<pdu_slice> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of pdu_utils::pdu_slice.
     *
     * @param slice - string that defines how PDU is sliced
     */
    static sptr make(std::string slice);

    /**
     * Sets slice notation
     *
     * @param slice
     */
    virtual void set_slice(std::string slice) = 0;
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_SLICE_H */
