/* -*- c++ -*- */
/*
 * Copyright 2018-2021 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
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
    typedef std::shared_ptr<pdu_pfb_resamp<T, S>> sptr;

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
