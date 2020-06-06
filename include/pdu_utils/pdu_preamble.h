/* -*- c++ -*- */
/*
 * <COPYRIGHT PLACEHOLDER>
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


#ifndef INCLUDED_PDU_UTILS_PDU_PREAMBLE_H
#define INCLUDED_PDU_UTILS_PDU_PREAMBLE_H

#include <pdu_utils/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace pdu_utils {

    /*!
     * \brief Adds a preamble and trailer to a PDU as well as performs interpolation and NRZ encoding
     * \ingroup pdu_utils
     *
     */
    class PDU_UTILS_API pdu_preamble : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<pdu_preamble> sptr;

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
      static sptr make(const std::vector<uint8_t> preamble, const std::vector<uint8_t> tail, uint32_t interp=1, uint32_t zero_pad=0, bool nrz=true);

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
