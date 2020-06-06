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

#ifndef INCLUDED_PDU_UTILS_PDU_PFB_RESAMP_CC_H
#define INCLUDED_PDU_UTILS_PDU_PFB_RESAMP_CC_H

#include <gnuradio/block.h>
#include <pdu_utils/api.h>

namespace gr {
namespace pdu_utils {

/*!
 * \brief PFB resample PDU
 * \ingroup pdu_utils
 *
 * This block will apply a PFB resampling kernel to input data.
 *
 */
template <class T, class S>
class PDU_UTILS_API pdu_pfb_resamp : virtual public gr::block
{
public:
    typedef boost::shared_ptr<pdu_pfb_resamp<T, S>> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of pdu_utils::pdu_pfb_resamp_cc.
     *
     * @param taps -
     * @param n_filters -
     * @param resamp_rate -
     */
    static sptr make(const std::vector<S> taps, int n_filters, float resamp_rate);

    /**
     * Set taps
     *
     * @param taps -
     */
    virtual void set_taps(std::vector<S> taps) = 0;
};

typedef pdu_pfb_resamp<float, float> pdu_pfb_resamp_fff;
typedef pdu_pfb_resamp<gr_complex, float> pdu_pfb_resamp_ccf;
typedef pdu_pfb_resamp<gr_complex, gr_complex> pdu_pfb_resamp_ccc;
} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_PFB_RESAMP_CC_H */
