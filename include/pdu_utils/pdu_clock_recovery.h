/* -*- c++ -*- */
/*
 * Copyright 2018, 2019, 2020 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */


#ifndef INCLUDED_PDU_UTILS_PDU_CLOCK_RECOVERY_H
#define INCLUDED_PDU_UTILS_PDU_CLOCK_RECOVERY_H

#include "constants.h"
#include <gnuradio/block.h>
#include <pdu_utils/api.h>

namespace gr {
namespace pdu_utils {

/*!
 * \brief Clock recovery based off whole pdu content
 * \ingroup pdu_utils
 *
 * Performs whole-packet clock recovery on a PDU.
 * Based on Michael Ossmann's talk here: https://www.youtube.com/watch?v=rQkBDMeODHc
 *
 * Input: demodulated packet, any number of samples per symbol, f32vector
 *
 * Output: f32vector PDU of soft symbols if slice is set to none, u8vector PDU of symbols
 * if slice is set to binary
 *
 * Meta Data: Removes sample rate, Inserts symbol rate
 *
 * Debug port debug = FFT of the Windowed Zero Crossing waveform
 *
 * Debug port zeroX = zero Crossing waveform. This places a sinc wave every spot the input
 * sample has a zero crossing
 *
 * Debug port window = windowed version of zeroX
 *
 */
class PDU_UTILS_API pdu_clock_recovery : virtual public gr::block
{
public:
    typedef boost::shared_ptr<pdu_clock_recovery> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of pdu_utils::pdu_clock_recovery.
     *
     * @param binary_slice - true if binary slicing to produce a u8vector
     * @param debug - true to enable debug ports & logging
     * @param type - window type to use.
     */
    static sptr make(bool binary_slice, bool debug = false, window_type type = TUKEY_WIN);

    /**
     * Specify what window type to use.
     *
     *
     * @param type - Window type to use
     */
    virtual void set_window_type(window_type type) = 0;
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_CLOCK_RECOVERY_H */
