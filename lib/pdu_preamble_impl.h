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

#ifndef INCLUDED_PDU_UTILS_PDU_PREAMBLE_IMPL_H
#define INCLUDED_PDU_UTILS_PDU_PREAMBLE_IMPL_H

#include <pdu_utils/pdu_preamble.h>
#include <pdu_utils/constants.h>

namespace gr {
  namespace pdu_utils {

    class pdu_preamble_impl : public pdu_preamble
    {
     private:
      std::vector<uint8_t> d_preamble;
      std::vector<float> d_preamble_interp;
      std::vector<uint8_t> d_tail;
      std::vector<float> d_tail_interp;
      std::vector<uint8_t> d_zeros;
      uint32_t d_interp;
      uint32_t d_zero_pad;
      bool d_nrz;

     public:

      /**
       * Constructor
       *
       * @param preamble - array to be inserted at the begining of the PDU data
       * @param tail - array to be inserted after the PDU data
       * @param interp - interpolation factor of each value, default = 1
       * @param zero_pad - number of zeros inserted at the start and end of the PDU, default = 0
       * @param nrz - Non Return to Zero encoding mode, default = true
       */
      pdu_preamble_impl(const std::vector<uint8_t> preamble, const std::vector<uint8_t> tail, uint32_t interp=1, uint32_t zero_pad=0, bool nrz=true);

      /**
       * Deconstructor
       */
      virtual ~pdu_preamble_impl();

      void handle_msg(pmt::pmt_t pdu);

      /**
       * Set the preamble array
       *
       * @param preamble - preamble array thats inserted before PDU
       */
      void set_preamble(const std::vector<uint8_t> preamble);

      /**
       * Set the tail array
       *
       * @param tail - tail array thats inserted after the PDU
       */
      void set_tail(const std::vector<uint8_t> tail);

      /**
       * Sets the interpolation factor
       *
       * @param interp - desired interpolation value
       */
      void set_interp(uint32_t interp);

      /**
       * Sets the number of zeros pading the front and rear of the encoded PDU
       *
       * @param zero_pad - number of zeros to pad with
       */
      void set_zero_pad(uint32_t zero_pad);

      /**
       * Sets the Non Return to Zero( NRZ ) behavior
       *
       * @param nrz - desired nrz value
       */
      void set_nrz(bool nrz);
    };

  } // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_PREAMBLE_IMPL_H */
