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

#ifndef INCLUDED_PDU_UTILS_PDU_CLOCK_RECOVERY_IMPL_H
#define INCLUDED_PDU_UTILS_PDU_CLOCK_RECOVERY_IMPL_H

#include <pdu_utils/pdu_clock_recovery.h>
#include <pdu_utils/constants.h>
#include <gnuradio/fft/fft.h>

const int LUT_SIZE = 256;

namespace gr {
  namespace pdu_utils {

    class pdu_clock_recovery_impl : public pdu_clock_recovery
    {
     private:
      void pdu_handler(pmt::pmt_t pdu);
      float midpoint(const float* data, size_t len);
      float median(const float* data, size_t len);
      float fast_sinc(float x);
      void init_fast_sinc();

      const int d_lanczos_a;
      const float d_gauss_sigma;
      float d_sinc_table[LUT_SIZE];

      const float d_dc_reject;

      void fft_setup(int power);
      void fft_cleanup();
      std::vector<gr::fft::fft_real_fwd*> d_ffts;
      std::vector<float*> d_windows;
      float* d_mags;

      bool d_binary_slice;

     public:
      pdu_clock_recovery_impl(bool binary_slice);
      ~pdu_clock_recovery_impl();
    };

  } // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_CLOCK_RECOVERY_IMPL_H */

