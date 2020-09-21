/* -*- c++ -*- */
/*
 * Copyright 2018, 2019, 2020 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_PDU_UTILS_PDU_QUADRATURE_DEMOD_CF_H
#define INCLUDED_PDU_UTILS_PDU_QUADRATURE_DEMOD_CF_H

#include <gnuradio/block.h>
#include <pdu_utils/api.h>

namespace gr {
namespace pdu_utils {

/*!
 * \brief PDU Quadrature Demod
 * \ingroup pdu_utils
 *
 * c32vector input, f32vector output
 */
class PDU_UTILS_API pdu_quadrature_demod_cf : virtual public gr::block
{
public:
    typedef boost::shared_ptr<pdu_quadrature_demod_cf> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of pdu_utils::pdu_quadrature_demod_cf.
     *
     * @param sensitivity -
     */
    static sptr make(float sensitivity);
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_QUADRATURE_DEMOD_CF_H */
