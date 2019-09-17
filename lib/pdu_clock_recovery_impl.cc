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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include <volk/volk.h>
#include "pdu_clock_recovery_impl.h"

#include <algorithm>

namespace gr {
  namespace pdu_utils {

    pdu_clock_recovery::sptr
    pdu_clock_recovery::make(bool binary_slice)
    {
      return gnuradio::get_initial_sptr
        (new pdu_clock_recovery_impl(binary_slice));
    }

    /*
     * The private constructor
     */
    pdu_clock_recovery_impl::pdu_clock_recovery_impl(bool binary_slice)
      : gr::block("pdu_clock_recovery",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0)),
        d_binary_slice(binary_slice),
        d_lanczos_a(1), // logic for adding pulses currently relies on this being 1
        d_gauss_sigma(1.0f/8.0f),
        d_dc_reject(0.05),
        d_mags(nullptr)
    {
      message_port_register_in(PMTCONSTSTR__PDU_IN);
      message_port_register_out(PMTCONSTSTR__PDU_OUT);
      message_port_register_out(pmt::mp("debug"));
      set_msg_handler(PMTCONSTSTR__PDU_IN, boost::bind(&pdu_clock_recovery_impl::pdu_handler, this, _1));
      init_fast_sinc();
      fft_setup(15);
    }

    /*
     * Our virtual destructor.
     */
    pdu_clock_recovery_impl::~pdu_clock_recovery_impl()
    {
      fft_cleanup();
    }

    // return median value
    float pdu_clock_recovery_impl::median(const float* data, size_t len) {
      if (len % 2 == 0) {
        return (data[len/2] + data[len/2-1]) / 2.0;
      }
      else {
        return data[len/2];
      }
    }

    // return midpoint of data (average of medians of above-average and below-average values)
    float pdu_clock_recovery_impl::midpoint(const float* data, size_t len) {
      std::vector<float> vec_data(data, data+len);
      std::sort(vec_data.begin(), vec_data.end());
      float mean = 0;
      for (float item : vec_data) {
        mean += item;
      }
      mean /= len;

      int mid_idx;
      for (mid_idx=0; mid_idx<len; mid_idx++) {
        if (vec_data[mid_idx] > mean) {
          break;
        }
      }
      float lower_median = median(vec_data.data(), len - mid_idx-1);
      float upper_median = median(vec_data.data()+mid_idx, len - mid_idx);
      return (upper_median + lower_median) / 2.0f;
    }

    void pdu_clock_recovery_impl::fft_setup(int power) {
      for (int i=d_ffts.size(); i<=power; i++) {

        // init fft
        int fftsize = pow(2, i);
        d_ffts.push_back(new gr::fft::fft_real_fwd(fftsize, 1));

        // init window
        d_windows.push_back((float*)volk_malloc(sizeof(float)*fftsize, volk_get_alignment()));
        float two_sigma_squared = fftsize * d_gauss_sigma;
        two_sigma_squared *= 2 * two_sigma_squared;
        for (int j=0; j<fftsize; j++) {
          float x = (-fftsize+1)/2.0f + j;
          d_windows[i][j] = std::exp((-x*x) / two_sigma_squared);
        }

        // init d_mags
        if (d_mags != nullptr) {
          volk_free(d_mags);
        }
        d_mags = (float*) volk_malloc(sizeof(float)*fftsize, volk_get_alignment());

      }
    }

    void pdu_clock_recovery_impl::fft_cleanup() {
      for (gr::fft::fft_real_fwd* fft : d_ffts) {
        delete fft;
      }
      d_ffts.clear();
      for (float* win : d_windows) {
        volk_free(win);
      }
      d_windows.clear();
      if (d_mags != nullptr) {
        volk_free(d_mags);
      }
      d_mags = nullptr;
    }

    // initialize LUT for a lanczos windowed sinc function
    // note: LUT is one-sided to take advantage of symmetry
    void pdu_clock_recovery_impl::init_fast_sinc() {
      const float pi_squared = M_PI * M_PI;
      d_sinc_table[0] = 1.0;
      for (int i=1; i<LUT_SIZE; i++) {
        float x = i * d_lanczos_a / (float)(LUT_SIZE-1);
        d_sinc_table[i] = d_lanczos_a * std::sin(M_PI*x) * std::sin(M_PI*x/d_lanczos_a) / (pi_squared * x * x);
      }
    }

    // fast sinc table lookup (lanczos windowed sinc)
    float pdu_clock_recovery_impl::fast_sinc(float x) {
      x = fabs(x);
      if (x >= d_lanczos_a) {
        return 0.0f;
      }
      x *= (LUT_SIZE-1) / (float)d_lanczos_a;
      int x_upper = std::ceil(x);
      int x_lower = std::floor(x);
      float mu = x - x_lower;
      // linearly interpolate between table entries
      return d_sinc_table[x_lower] + mu * (d_sinc_table[x_upper] - d_sinc_table[x_lower]);
    }

    void pdu_clock_recovery_impl::pdu_handler(pmt::pmt_t pdu) {
      if (!pmt::is_pair(pdu)) {
        GR_LOG_WARN(d_logger, "PDU is not a pair, dropping\n");
        return;
      }

      pmt::pmt_t metadata = pmt::car(pdu);
      pmt::pmt_t pdu_data = pmt::cdr(pdu);

      if (!pmt::is_f32vector(pdu_data)) {
        GR_LOG_WARN(d_logger, "PDU is not f32vector, dropping\n");
        return;
      }

      if (!pmt::is_dict(metadata)) {
        GR_LOG_WARN(d_logger, "PDU metadata is not dict, dropping\n");
        return;
      }

      pmt::pmt_t pmt_samp_rate = pmt::dict_ref(metadata, pmt::mp("sample_rate"), pmt::get_PMT_NIL());
      if (pmt_samp_rate == pmt::get_PMT_NIL() || !pmt::is_number(pmt_samp_rate)) {
        GR_LOG_WARN(d_logger, "no sample rate, dropping\n");
        return;
      }
      float samp_rate = pmt::to_float(pmt_samp_rate);

      size_t length;
      const float* data = pmt::f32vector_elements(pdu_data, length);

      //float mid = midpoint(data, length);
      const float mid = 0;

      // make a list of zero crossing locations
      std::vector<float> zero_crossings;
      zero_crossings.reserve(length/2); // make a generous estimate of how many zero crossings there are
      for (int i=1; i<length; i++) {
        if ((data[i] > mid) != (data[i-1] > mid)) {
          zero_crossings.push_back(i - 1 + (data[i-1] - mid) / (data[i-1] - data[i]));
        }
      }

      if (zero_crossings.empty()) {
        GR_LOG_WARN(d_logger, "no zero crossings found, dropping");
        return;
      }

      // fftsize is the data length rounded up to a power of 2
      int fftpower = std::ceil(log2(length));
      int fftsize = pow(2, fftpower);
      fft_setup(fftpower);

      float* fft_in = d_ffts[fftpower]->get_inbuf();

      // set fft input to 0
      memset(fft_in, 0, sizeof(float)*fftsize);

      // drop sinc pulses at every zero crossing location
      for (const float &crossing : zero_crossings) {
        int idx = round(crossing);
        if (idx == crossing) {
          fft_in[idx] = 1.0f;
        } else if (idx > 0 && idx < fftsize-1) {
          // XXX: this assumes d_lanczos_a == 1
          fft_in[idx-1] += fast_sinc(idx - 1 - crossing);
          fft_in[idx]   += fast_sinc(idx     - crossing);
          fft_in[idx+1] += fast_sinc(idx + 1 - crossing);
        }
      }

      // apply gaussian window
      volk_32f_x2_multiply_32f(fft_in, fft_in, d_windows[fftpower], length);

      d_ffts[fftpower]->execute();
      fftsize /= 2; // real transform only outputs positive frequencies

      // get magnitudes squared from fft output
      gr_complex* fft_out = d_ffts[fftpower]->get_outbuf();
      volk_32fc_magnitude_squared_32f(d_mags, fft_out, fftsize);

      message_port_pub(pmt::mp("debug"), pmt::init_f32vector(fftsize, d_mags));
      
      // get largest magnitude
      int first_bin = std::ceil(d_dc_reject * fftsize);
      float max_val = -1;
      int max_bin = 0;
      for (int i=first_bin; i<fftsize; i++) {
        if (d_mags[i] > max_val) {
          max_val = d_mags[i];
          max_bin = i;
        }
      }

      // make sure the largest magnitude wasn't a harmonic of the frequency we want
      float thresh = 0.5*d_mags[max_bin];
      // 'denom' is the harmonic we are checking (if denom is 5, we're checking if max_bin is a 5th harmonic)
      for (int denom=5; denom>=2; denom--) {
        bool match = true;
        for (int num=1; num<denom; num++) {
          // check if 1/denom, 2/denom, ..., (denom-1)/denom are all above the threshold
          int idx = round(num/(float)denom * max_bin);
          if (idx < first_bin) {
            match = false;
            break;
          }
          if (d_mags[idx]   < thresh &&
              d_mags[idx-1] < thresh &&
              d_mags[idx+1] < thresh) {
            match = false;
            break;
          }
        }
        // if it was a harmonic, set max_bin to the fundamental
        if (match) {
          // set max_bin to the largest of the three bins
          //printf("matched max_bin %d with denom %d\n", max_bin, denom);
          max_bin = round(1.0f/denom * max_bin) - 1;
          max_val = d_mags[max_bin];
          for (int i=max_bin+1; i<=max_bin+2; i++) {
            if (d_mags[i] > max_val) {
              max_val = d_mags[i];
              max_bin = i;
            }
          }
          break;
        }
      }

      float peak_bin = max_bin;
      if (max_bin < fftsize-1) {
        // https://ccrma.stanford.edu/~jos/sasp/Quadratic_Interpolation_Spectral_Peaks.html
        float alpha = log(d_mags[max_bin-1]);
        float beta  = log(d_mags[max_bin]);
        float gamma = log(d_mags[max_bin+1]);

        float bin_offset = 0.5f * (alpha - gamma) / (alpha - 2*beta + gamma);
        peak_bin = max_bin + bin_offset;
      }

      if (std::isnan(peak_bin) || std::isinf(peak_bin)) {
        GR_LOG_WARN(d_logger, "peak_bin is inf or nan, dropping");
        return;
      }

      // interpolate real and imaginary peaks separately to get accurate phase
      int n_pts_to_interp = std::min(max_bin, 5);
      int maxi = std::min(max_bin + n_pts_to_interp, fftsize - 1);
      float real_peak = 0;
      float imag_peak = 0;
      for (int i=max_bin - n_pts_to_interp; i<= maxi; i++) {
        float sinc;
        if (i - peak_bin == 0) {
          sinc = 1;
        } else {
          sinc = std::sin(M_PI*(i - peak_bin)) / (M_PI*(i - peak_bin));
        }
        real_peak += std::real(fft_out[i]) * sinc;
        imag_peak += std::imag(fft_out[i]) * sinc;
      }

      // calculate clock phase and symbol rate
      float phase = std::arg(gr_complex(real_peak, imag_peak));

      float symbol_freq = peak_bin / fftsize / 2.0f;
      float clock_phase = phase / (2*M_PI);
      clock_phase += 0.5f; // we want sample times, not zero crossing times
      if (clock_phase <= 0) {
        clock_phase += 1.0f;
      }

      // now extract soft symbols
      std::vector<float> symbols;
      try {
        symbols.reserve((int) length * symbol_freq + 100);
      } catch (const std::exception& e) {
        GR_LOG_WARN(d_logger, e.what());
        GR_LOG_WARN(d_logger, "Failed to reserve space for symbols, dropping PDU");
        return;
      }
      clock_phase += symbol_freq;
      for (int i=1; i<length; i++) {
        if (clock_phase >= 1) {
          clock_phase -= 1.0f;
          float mu = clock_phase / symbol_freq;
          float interp = mu * data[i-1] + (1 - mu) * data[i];
          symbols.push_back(interp);
        }
        clock_phase += symbol_freq;
      }

      pmt::pmt_t data_vec;

      if (d_binary_slice) {
        std::vector<uint8_t> sym(symbols.size());
        std::transform(symbols.begin(), symbols.end(),
                       sym.begin(),
                       [&](float f) -> uint8_t { return (f > mid); });
        data_vec = pmt::init_u8vector(sym.size(), sym.data());
      } else {
        data_vec = pmt::init_f32vector(symbols.size(), symbols.data());
      }

      metadata = pmt::dict_delete(metadata, pmt::mp("sample_rate"));
      metadata = pmt::dict_add(metadata, pmt::mp("symbol_rate"), pmt::from_float(symbol_freq*samp_rate));

      message_port_pub(PMTCONSTSTR__PDU_OUT, pmt::cons(metadata, data_vec));
    }

  } /* namespace pdu_utils */
} /* namespace gr */

