/* -*- c++ -*- */
/*
 * Copyright 2018-2021 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_PDU_UTILS_PDU_FIR_FILTER_H
#define INCLUDED_PDU_UTILS_PDU_FIR_FILTER_H

#include <gnuradio/block.h>
#include <gnuradio/pdu_utils/api.h>

namespace gr {
namespace pdu_utils {

/*!
 * \brief PDU FIR filter
 * \ingroup pdu_utils
 *
 * This block will apply a FIR filter to an input PDU (byte, float, or complex)
 *
 */
class PDU_UTILS_API pdu_fir_filter : virtual public gr::block
{
public:
    typedef std::shared_ptr<pdu_fir_filter> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of pdu_utils::pdu_fir_filter.
     *
     * @param decimation - decimation factor to apply
     * @param taps - FIR taps
     */
    static sptr make(int decimation, const std::vector<float> taps);

    /**
     * Set FIR taps
     *
     * @param taps - FIR taps
     */
    virtual void set_taps(std::vector<float> taps) = 0;

    /**
     * Set Decimation factor
     *
     * @param decimation - decimation factor
     */
    virtual void set_decimation(int decimation) = 0;
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_FIR_FILTER_H */
