/* -*- c++ -*- */
/*
 * Copyright 2018 National Technology & Engineering Solutions of Sandia, LLC (NTESS).
 * Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government retains
 * certain rights in this software.
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

#ifndef INCLUDED_PDU_UTILS_PDU_GMSK_FC_IMPL_H
#define INCLUDED_PDU_UTILS_PDU_GMSK_FC_IMPL_H

#include <gnuradio/filter/fir_filter.h>
#include <pdu_utils/constants.h>
#include <pdu_utils/pdu_gmsk_fc.h>

namespace gr {
namespace pdu_utils {

class pdu_gmsk_fc_impl : public pdu_gmsk_fc
{
private:
    float d_sensitivity;
    float d_phase;
    std::vector<float> d_taps;
    filter::kernel::fir_filter_fff* d_fir;
    std::vector<float> d_log_ramp;

public:
    /**
     * constructor
     *
     * @param sensitivity -
     * @param taps -
     */
    pdu_gmsk_fc_impl(float sensitivity, const std::vector<float> taps);

    /**
     * Deconstructor
     */
    ~pdu_gmsk_fc_impl();

    void handle_pdu(pmt::pmt_t pdu);

    /**
     * Sets sensitivity
     *
     * @param sensitivity -
     */
    void set_sensitivity(float sensitivity);

    /**
     * Sets taps array
     *
     * @param taps -
     */
    void set_taps(std::vector<float> taps);
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_GMSK_FC_IMPL_H */
