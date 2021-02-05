/* -*- c++ -*- */
/*
 * Copyright 2018-2021 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_PDU_UTILS_PDU_ROTATE_H
#define INCLUDED_PDU_UTILS_PDU_ROTATE_H

#include <gnuradio/block.h>
#include <pdu_utils/api.h>

namespace gr {
namespace pdu_utils {

/*!
 * \brief <+description of block+>
 * \ingroup pdu_utils
 *
 */
class PDU_UTILS_API pdu_rotate : virtual public gr::block
{
public:
    typedef std::shared_ptr<pdu_rotate> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of pdu_utils::pdu_rotate.
     *
     * @param phase_inc - phase increment in radians
     */
    static sptr make(double phase_inc);

    /**
     * Set Phase Increment
     *
     * @param phase_inc - Phase Increment
     */
    virtual void set_phase_inc(double phase_inc) = 0;
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_ROTATE_H */
