/* -*- c++ -*- */
/* 
 * Copyright 2018 <+YOU OR YOUR COMPANY+>.
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

namespace gr
{
  namespace pdu_utils
  {

    /**
     * Implementation class for pdu_clock_recovery
     */
    class pdu_clock_recovery_impl : public pdu_clock_recovery
    {
      private:
        const int d_lanczos_a;
        const float d_gauss_sigma;
        float d_sinc_table[LUT_SIZE];

        const float d_dc_reject;

        std::vector<gr::fft::fft_real_fwd*> d_ffts;
        std::vector<float*> d_windows;
        float *d_mags;

        bool d_binary_slice;
        bool d_debug;

      public:

        /**
         * Constructor
         *
         * @param binary_slice - true if binary slicing to produce a u8vector
         * @param debug - true to enable debug ports and logging
         */
        pdu_clock_recovery_impl( bool binary_slice, bool debug=false );

        /**
         * deconstructor
         */
        virtual ~pdu_clock_recovery_impl();

      private:
        /**
         * Handles PDUs from pdu_in port
         * Expects input pdu to meet the following criteria
         * - PMT pair, car = dict, cdr = data
         * - data is a f32vector
         * - dict is a dictionary
         * - dict contains a key, sample_rate, representing the sample rate of the incoming data
         *
         * @param pdu - PMT pair of dict & data
         */
        void pdu_handler( pmt::pmt_t pdu );

        /**
         * fast sinc table lookup( lanczos windows sinc )
         *
         * @param x -
         * @return float -
         */
        float fast_sinc( float x );

        /**
         * initialize LUT for a lanczos windowed sinc function
         * note: LUT is one-sided to take advantage of symmetryS
         */
        void init_fast_sinc();

        /**
         * sets up FFT memory space for a given power if needed
         *
         * @param power - power of FFT
         */
        void fft_setup( int power );

        /**
         * cleans up all memory associated with FFTs, windows, & mags
         */
        void fft_cleanup();

        /**
         * Calculates the gaussian window value at a specific position
         *
         * @param n - total number of samples to calculate over
         * @param sigma - sigma value
         * @param x - index/sample to calculate for
         * @return float - gaussianWindows at x
         */
        float gaussianWindow( int n, float sigma, int x );

        /**
         * finds zero crossings in the data. Returns a vector of indexes
         * where zero crossings happen
         *
         * @param data - pointer to real waveform
         * @param len - length of data
         * @return std::vector<float> - list of zero crossing points
         */
        std::vector<float> findZeroCrossings( const float *data, const int len );

        /**
         * Does a bunch  of pre-condition checks on the input PDU
         *
         * @param pdu - incoming PDU
         * @return bool - true if pre-conditions met
         */
        bool inputCheck( pmt::pmt_t pdu );

        /**
         * Generates a sync pulse waveform based on zero crossing positions.
         * This function also selects a portion of the original sample set's
         * zero crossings to use for the waveform. This resolves the fact
         * that the FFT size will always be smaller than the sample size
         *
         * @param crossings - locations of zero crossings
         * @param sampLen - original length of sample set
         * @param out - storage for sync waveform
         * @param len - max size of storage
         * @return int - startOffset, offset used from original input sample reference to Sinc/FFT reference
         */
        int genSincWaveform( std::vector<float> crossings, const int sampLen, float *out, const int len );

        /**
         * Finds the max fundamental freq present in the FFT magnitude array.
         *
         * @param mags - FFT magnitude
         * @param len - length of mags
         * @return int - index of mags representing the max fundamental freq
         */
        int findMaxFundamental( const float *mags, const int len );

        /**
         * Interpolates peak bin position
         *
         * @param mags - FFT magnitude
         * @param len - length of mags
         * @param max_bin - index into mags with max value
         * @return float - calculated peak bin
         */
        float calcPeakBin( const float *mags, const int len, const int max_bin );

        /**
         * Calculate Peak phase
         *
         * @param fft - complex FFT data
         * @param fftsize - size of fft
         * @param max_bin - index of max bin
         * @param peak_bin - calculated peak bin
         * @return float - phase
         */
        float calcPeakPhase( gr_complex *fft, const int fftsize, const int max_bin, const float peak_bin );

        /**
         * Extracts symbols from the original input data.
         *
         * @param data - input data buffer
         * @param len - length of data
         * @param symbol_freq - calculated symbol frequency
         * @param phase - calculated phase of symbol frequency
         * @param offset - sample offset used by genSincWaveform. Used for phase correction
         * @return std::vector<float> - vector of symbols
         */
        std::vector<float> extractSymbols( const float *data, const int len, const float symbol_freq, const float phase, const int offset );

    }; //end class pdu_clock_recovery_impl

  } // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_CLOCK_RECOVERY_IMPL_H */

