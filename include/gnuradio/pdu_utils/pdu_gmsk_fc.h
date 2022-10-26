/* -*- c++ -*- */
/*
 * Copyright 2018-2021 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_PDU_UTILS_PDU_GMSK_FC_H
#define INCLUDED_PDU_UTILS_PDU_GMSK_FC_H

#include <gnuradio/block.h>
#include <gnuradio/pdu_utils/api.h>
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
    typedef std::shared_ptr<pdu_gmsk_fc> sptr;

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
