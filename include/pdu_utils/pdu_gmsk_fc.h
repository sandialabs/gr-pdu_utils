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

#ifndef INCLUDED_PDU_UTILS_PDU_GMSK_FC_H
#define INCLUDED_PDU_UTILS_PDU_GMSK_FC_H

#include <gnuradio/block.h>
#include <pdu_utils/api.h>
//#include <gnuradio/sync_block.h>

namespace gr {
namespace pdu_utils {

/*!
 * \brief GMSK Modulator
 * \ingroup pdu_utils
 *
 * This block accepts a float PDU of size n containing baseband
 * data, applies GMSK modulation, then sends a gr_complex PDU of
 * size n-len(taps)+1.
 */
class PDU_UTILS_API pdu_gmsk_fc : virtual public gr::block
{
public:
    typedef boost::shared_ptr<pdu_gmsk_fc> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of pdu_utils::pdu_gmsk_fc.
     *
     * @param sensitivity -
     * @param taps -
     */
    static sptr make(float sensitivity, const std::vector<float> taps);

    /**
     * Sets sensitivity
     *
     * @param sensitivity -
     */
    virtual void set_sensitivity(float sensitivity) = 0;

    /**
     * Sets taps array
     *
     * @param taps -
     */
    virtual void set_taps(std::vector<float> taps) = 0;
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_GMSK_FC_H */
