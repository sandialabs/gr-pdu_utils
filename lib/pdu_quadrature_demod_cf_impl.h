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

#ifndef INCLUDED_PDU_UTILS_PDU_QUADRATURE_DEMOD_CF_IMPL_H
#define INCLUDED_PDU_UTILS_PDU_QUADRATURE_DEMOD_CF_IMPL_H

#include <gnuradio/filter/fir_filter.h>
#include <pdu_utils/pdu_quadrature_demod_cf.h>

namespace gr {
namespace pdu_utils {

class pdu_quadrature_demod_cf_impl : public pdu_quadrature_demod_cf
{
private:
    float d_sensitivity;

public:
    /**
     * Constructor
     *
     * @param sensitivity -
     */
    pdu_quadrature_demod_cf_impl(float sensitivity);

    /**
     * Deconstructor
     */
    ~pdu_quadrature_demod_cf_impl();

    void handle_pdu(pmt::pmt_t pdu);
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_QUADRATURE_DEMOD_CF_IMPL_H */
