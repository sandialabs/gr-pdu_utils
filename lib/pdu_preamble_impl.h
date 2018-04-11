/* -*- c++ -*- */
/*
 * Copyright 2018 National Technology & Engineering Solutions of Sandia, LLC (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government retains certain rights in this software.
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
      pdu_preamble_impl(const std::vector<uint8_t> preamble, const std::vector<uint8_t> tail, uint32_t interp=1, uint32_t zero_pad=0, bool nrz=true);
      ~pdu_preamble_impl();

      void handle_msg(pmt::pmt_t pdu);

      void set_preamble(const std::vector<uint8_t> preamble);
      void set_tail(const std::vector<uint8_t> tail);
      void set_interp(uint32_t interp);
      void set_zero_pad(uint32_t zero_pad);
      void set_nrz(bool nrz);
    };

  } // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_PREAMBLE_IMPL_H */
