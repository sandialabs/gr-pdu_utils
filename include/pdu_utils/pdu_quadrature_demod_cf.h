/* -*- c++ -*- */
/*
 * Copyright 2020 gr-pdu_utils author.
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
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
