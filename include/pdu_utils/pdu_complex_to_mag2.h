/* -*- c++ -*- */
/*
 * Copyright 2018-2021 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#ifndef INCLUDED_PDU_UTILS_PDU_COMPLEX_TO_MAG2_H
#define INCLUDED_PDU_UTILS_PDU_COMPLEX_TO_MAG2_H

#include <gnuradio/block.h>
#include <pdu_utils/api.h>

namespace gr {
namespace pdu_utils {

/*!
 * \brief converts Complex PDUs to Float Magnitude PDUs
 * \ingroup pdu_utils
 *
 * Converts a c32vector PDU to a f32vector PDU using magnitude squared
 */
class PDU_UTILS_API pdu_complex_to_mag2 : virtual public gr::block
{
public:
    typedef std::shared_ptr<pdu_complex_to_mag2> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of pdu_utils::pdu_complex_to_mag2.
     *
     *
     */
    static sptr make();
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_COMPLEX_TO_MAG2_H */
