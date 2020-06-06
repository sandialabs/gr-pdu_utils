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

#ifndef INCLUDED_PDU_UTILS_PDU_PFB_RESAMP_FF_IMPL_H
#define INCLUDED_PDU_UTILS_PDU_PFB_RESAMP_FF_IMPL_H

#include <pdu_utils/pdu_pfb_resamp_ff.h>
#include <pdu_utils/constants.h>
#include <gnuradio/filter/pfb_arb_resampler.h>

namespace gr {
  namespace pdu_utils {

    class pdu_pfb_resamp_ff_impl : public pdu_pfb_resamp_ff
    {
     private:
      std::vector<float> d_taps;
      filter::kernel::pfb_arb_resampler_fff *d_pfb;
      uint32_t d_nfilts;
      float d_resamp_rate;

     public:
      pdu_pfb_resamp_ff_impl(const std::vector<float> taps, int n_filters, float resamp_rate);
      ~pdu_pfb_resamp_ff_impl();

      void handle_pdu(pmt::pmt_t pdu);

      void set_taps(std::vector<float> taps) { d_taps = taps; };
    };

  } // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_PFB_RESAMP_FF_IMPL_H */
