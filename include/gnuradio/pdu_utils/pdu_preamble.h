/* -*- c++ -*- */
/*
 * Copyright 2018-2021 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */


#ifndef INCLUDED_PDU_UTILS_PDU_PREAMBLE_H
#define INCLUDED_PDU_UTILS_PDU_PREAMBLE_H

#include <gnuradio/block.h>
#include <gnuradio/pdu_utils/api.h>

namespace gr {
namespace pdu_utils {

/*!
 * \brief Adds a preamble and trailer to a PDU as well as performs interpolation and NRZ
 * encoding \ingroup pdu_utils
 *
 * This block will add a preamble, tail, interpolate, NRZ encode, and zero pad according
 * to the users specification.
 *
 */
class PDU_UTILS_API pdu_preamble : virtual public gr::block
{
public:
    typedef std::shared_ptr<pdu_preamble> sptr;

    /**
     * Creates a pdu_preamble and returns a shared pointer
     *
     * @param preamble - array to be inserted at the begining of the PDU data
     * @param tail - array to be inserted after the PDU data
     * @param interp - interpolation factor of each value
     * @param zero_pad - number of zeros inserted at the start and end of the PDU
     * @param nrz - Non Return to Zero encoding mode
     * @return pdu_preamble::sptr - shared pointer to pdu_preamble instance
     */
    static sptr make(const std::vector<uint8_t> preamble,
                     const std::vector<uint8_t> tail,
                     uint32_t interp = 1,
                     uint32_t zero_pad = 0,
                     bool nrz = true);

    /**
     * Set the preamble array
     *
     * @param preamble - preamble array thats inserted before PDU
     */
    virtual void set_preamble(const std::vector<uint8_t> preamble) = 0;

    /**
     * Set the tail array
     *
     * @param tail - tail array thats inserted after the PDU
     */
    virtual void set_tail(const std::vector<uint8_t> tail) = 0;


    /**
     * Sets the interpolation factor
     *
     * @param interp - desired interpolation value
     */
    virtual void set_interp(uint32_t interp) = 0;


    /**
     * Sets the number of zeros pading the front and rear of the encoded PDU
     *
     * @param zero_pad - number of zeros to pad with
     */
    virtual void set_zero_pad(uint32_t zero_pad) = 0;

    /**
     * Sets the Non Return to Zero( NRZ ) behavior
     *
     * @param nrz - desired nrz value
     */
    virtual void set_nrz(bool nrz) = 0;
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_PREAMBLE_H */
