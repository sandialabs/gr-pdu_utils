/* -*- c++ -*- */
/*
 * Copyright 2018-2021 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "pdu_clock_recovery_impl.h"
#include <gnuradio/io_signature.h>
#include <volk/volk.h>
#include <boost/format.hpp>
#include <algorithm>

namespace gr {
namespace pdu_utils {

/*!
 * \brief Return a shared_ptr to a new instance of pdu_utils::pdu_clock_recovery.
 *
 * To avoid accidental use of raw pointers, pdu_utils::pdu_clock_recovery's
 * constructor is in a private implementation
 * class. pdu_utils::pdu_clock_recovery::make is the public interface for
 * creating new instances.
 *
 * @param binary_slice - true if binary slicing to produce a u8vector
 * @param debug - true to enable debug ports & logging
 * @param type - window type to use.
 */
pdu_clock_recovery::sptr
pdu_clock_recovery::make(bool binary_slice, bool debug, window_type type)
{
    return gnuradio::make_block_sptr<pdu_clock_recovery_impl>(binary_slice, debug, type);
}

/**
 * Constructor
 *
 * @param binary_slice - true if binary slicing to produce a u8vector
 * @param debug - true to enable debug ports and logging
 * @param type - window type to use.
 */
pdu_clock_recovery_impl::pdu_clock_recovery_impl(bool binary_slice,
                                                 bool debug,
                                                 window_type type)
    : gr::block("pdu_clock_recovery",
                gr::io_signature::make(0, 0, 0),
                gr::io_signature::make(0, 0, 0)),
      d_binary_slice(binary_slice),
      d_lanczos_a(1), // logic for adding pulses currently relies on this being 1
      d_gauss_sigma(2.0f / 8.0f),
      d_dc_reject(0.05),
      d_mags(nullptr),
      d_debug(debug),
      d_burst_id(0),
      d_window_type(type)
{

    // setup ports
    message_port_register_in(PMTCONSTSTR__pdu_in());
    message_port_register_out(PMTCONSTSTR__pdu_out());
    if (d_debug) {
        message_port_register_out(PMTCONSTSTR__debug());
        message_port_register_out(PMTCONSTSTR__zeroX());
        message_port_register_out(PMTCONSTSTR__window());
    }
    set_msg_handler(PMTCONSTSTR__pdu_in(),
                    [this](pmt::pmt_t msg) { this->pdu_handler(msg); });

    init_fast_sinc();
    fft_setup(15);
} // end constructor

/*
 * Our virtual destructor.
 */
pdu_clock_recovery_impl::~pdu_clock_recovery_impl() { fft_cleanup(); }

/**
 * Specify what window type to use.
 *
 *
 * @param type - Window type to use
 */
void pdu_clock_recovery_impl::set_window_type(window_type type)
{
    d_window_type = type;

    d_logger->info("Changing Window type {}",d_window_type);

    // flush all the existing pre-made memory & recreate it.
    fft_cleanup();
    fft_setup(15);
} // end set_window_type

/**
 * Specify the variance of the Gaussian window.
 *
 *
 * @param gauss_sigma - standard deviation
 */
void pdu_clock_recovery_impl::set_gauss_sigma(float gauss_sigma)
{
    d_gauss_sigma = gauss_sigma;
}

/**
 * Specify the DC reject level
 *
 *
 * @param dc_reject - DC rejection
 */
void pdu_clock_recovery_impl::set_dc_reject(float dc_reject)
{
    d_dc_reject = dc_reject;
}

/**
 * sets up FFT memory space for a given power if needed
 *
 * @param power - power of FFT
 */
void pdu_clock_recovery_impl::fft_setup(int power)
{
    for (int i = d_ffts.size(); i <= power; i++) {

        // init fft
        int fftsize = pow(2, i);
        d_ffts.push_back(new gr::fft::fft_real_fwd(fftsize, 1));

        // init window
        d_windows.push_back(
            (float*)volk_malloc(sizeof(float) * fftsize, volk_get_alignment()));
        for (int j = 0; j < fftsize; j++) {
            switch (d_window_type) {
            case (GAUSSIAN_WIN): {
                d_windows[i][j] = gaussianWindow(fftsize, d_gauss_sigma, j);
                break;
            }
            case (TUKEY_WIN):
            default: {
                d_windows[i][j] = tukeyWindow(fftsize, d_gauss_sigma, j);
                break;
            }
            } // end switch( d_window_type

            // printf("%f ", d_windows[i][j] );
        } // end for(j
        // printf("\n");

        // init d_mags
        if (d_mags != nullptr) {
            volk_free(d_mags);
        }
        d_mags = (float*)volk_malloc(sizeof(float) * fftsize, volk_get_alignment());

    } // end for(i
} // end fft_setup


/**
 * Calculates the gaussian window value at a specific position
 *
 * @param n - total number of samples to calculate over
 * @param sigma - sigma value
 * @param x - index/sample to calculate for
 * @return float - gaussianWindows at x
 */
float pdu_clock_recovery_impl::gaussianWindow(int n, float sigma, int x)
{
    float ans;

    float two_sigma_squared = n * d_gauss_sigma;
    two_sigma_squared *= 2 * two_sigma_squared;
    float t = (-n + 1) / 2.0f + x;
    ans = std::exp((-t * t) / two_sigma_squared);

    return ans;
} // end gaussianWindow

/**
 * Calculates the Tukey window value at a specific position
 *
 * @param n - total number of samples to calculate over
 * @param alpha - alpha value
 * @param x - index/sample to calculate for
 * @return float - Tukey at x
 */
float pdu_clock_recovery_impl::tukeyWindow(int n, float alpha, int x)
{
    float ans = 0; // default

    // https://en.wikipedia.org/wiki/Window_function
    // printf("tukeyWindow( %d %f %d )\n", n, alpha, x );

    // which part of the waveform are we in?
    if (x == 0) {
        ans = 0;
    } else if (0 < x && x < (alpha * n / 2)) {
        // cosine slope
        ans = 0.5 * (1.0 - std::cos((2 * M_PI * x) / (alpha * n)));
    } else if ((alpha * n / 2) <= x && x <= (n / 2)) {
        // constant pass
        ans = 1.0;
    } else {
        // upper half, return answer from lower half
        ans = tukeyWindow(n, alpha, n - x);
    }


    return ans;
} // end tukeyWindow

/**
 * cleans up all memory associated with FFTs, windows, & mags
 */
void pdu_clock_recovery_impl::fft_cleanup()
{
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
} // end fft_cleanup

/**
 * initialize LUT for a lanczos windowed sinc function
 * note: LUT is one-sided to take advantage of symmetryS
 */
void pdu_clock_recovery_impl::init_fast_sinc()
{
    const float pi_squared = M_PI * M_PI;
    d_sinc_table[0] = 1.0;
    for (int i = 1; i < LUT_SIZE; i++) {
        float x = i * d_lanczos_a / (float)(LUT_SIZE - 1);
        d_sinc_table[i] = d_lanczos_a * std::sin(M_PI * x) *
                          std::sin(M_PI * x / d_lanczos_a) / (pi_squared * x * x);
    } // end for(i
}

/**
 * fast sinc table lookup( lanczos windows sinc )
 *
 * @param x -
 * @return float -
 */
float pdu_clock_recovery_impl::fast_sinc(float x)
{
    x = fabs(x);
    if (x >= d_lanczos_a) {
        return 0.0f;
    }
    x *= (LUT_SIZE - 1) / (float)d_lanczos_a;
    int x_upper = std::ceil(x);
    int x_lower = std::floor(x);
    float mu = x - x_lower;
    // linearly interpolate between table entries
    return d_sinc_table[x_lower] + mu * (d_sinc_table[x_upper] - d_sinc_table[x_lower]);
}

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
void pdu_clock_recovery_impl::pdu_handler(pmt::pmt_t pdu)
{
    int offset = 0; // input data offset for where to start processing

    // check input conditions
    if (inputCheck(pdu) == false) {
        return;
    }

    pmt::pmt_t metadata = pmt::car(pdu);
    pmt::pmt_t pdu_data = pmt::cdr(pdu);
    pmt::pmt_t pmt_samp_rate =
        pmt::dict_ref(metadata, PMTCONSTSTR__sample_rate(), pmt::get_PMT_NIL());
    float samp_rate = pmt::to_float(pmt_samp_rate);
    size_t length; // stores length of f32vector
    const float* data = pmt::f32vector_elements(pdu_data, length);

    // Setup Memory banks
    int fftpower = std::floor(log2(length));
    int fftsize = pow(2, fftpower);
    fft_setup(fftpower);
    float* fft_in = d_ffts[fftpower]->get_inbuf();
    memset(fft_in, 0, sizeof(float) * fftsize);
    if (d_debug) {
        d_burst_id = 0;
        if (pmt::dict_has_key(metadata, PMTCONSTSTR__burst_id())) {
            pmt::pmt_t id =
                pmt::dict_ref(metadata, PMTCONSTSTR__burst_id(), pmt::get_PMT_NIL());
            if (pmt::is_uint64(id)) {
                d_burst_id = pmt::to_uint64(id);
            }
        }

        d_logger->debug("BurstID {}, Input Sz: {}, FFT Power: {}, FFTSz: {}",
                         d_burst_id,length,fftpower,fftsize);
    }

    // calculate sample offset
    offset = (length - fftsize) / 2;
    if (d_debug) {
        d_logger->debug("BurstID {} sampLen:{} len:{} offset:{}",d_burst_id,length,fftsize,offset);
    }

    // make a list of zero crossing locations, offset
    std::vector<float> zero_crossings = findZeroCrossings(&data[offset], fftsize);
    if (zero_crossings.empty() || zero_crossings.size() < (size_t)(fftsize / (2 * SPS_MAX))) {
        if (d_debug) {
            d_logger->warn("BurstID {} no/low zero crossings found, dropping", d_burst_id);
        }
        return;
    }

    // metadata = pmt::dict_add( metadata, pmt::intern("clk_zerox_sz"), pmt::from_uint64(
    // zero_crossings.size() ) ); metadata = pmt::dict_add( metadata,
    // pmt::intern("clk_fft_sz"), pmt::from_uint64( fftsize ) );


    genSincWaveform(zero_crossings, length, fft_in, fftsize);
    if (d_debug) {
        message_port_pub(PMTCONSTSTR__zeroX(), pmt::init_f32vector(fftsize, fft_in));
    }

    // apply gaussian window
    volk_32f_x2_multiply_32f(fft_in, fft_in, d_windows[fftpower], fftsize);
    if (d_debug) {
        message_port_pub(PMTCONSTSTR__window(), pmt::init_f32vector(fftsize, fft_in));
        // message_port_pub( PMTCONSTSTR__window(), pmt::init_f32vector( fftsize,
        // d_windows[fftpower] ) );
    }

    // run the FFT
    d_ffts[fftpower]->execute();
    fftsize /= 2; // real transform only outputs positive frequencies
    gr_complex* fft_out = d_ffts[fftpower]->get_outbuf();
    volk_32fc_magnitude_squared_32f(d_mags, fft_out, fftsize);
    if (d_debug) {
        message_port_pub(PMTCONSTSTR__debug(), pmt::init_f32vector(fftsize, d_mags));
    }


    // Find fundamental max & associated info
    int max_bin = findMaxFundamental(d_mags, fftsize);
    float peak_bin = calcPeakBin(d_mags, fftsize, max_bin);
    float phase = calcPeakPhase(fft_out, fftsize, max_bin, peak_bin);

    float symbol_freq = peak_bin / fftsize / 2.0f;

    if (d_debug) {
        d_logger->debug("peak_bin {:e}   fftsize {}   symbol_freq {:e}    symbol_rate {:e}",
                peak_bin,fftsize,symbol_freq,(symbol_freq * samp_rate));
    }

    // now extract soft symbols
    std::vector<float> symbols = extractSymbols(data, length, symbol_freq, phase, offset);

    // format the output
    pmt::pmt_t data_vec;

    if (d_binary_slice) {
        std::vector<uint8_t> sym(symbols.size());
        std::transform(symbols.begin(),
                       symbols.end(),
                       sym.begin(),
                       [&](float f) -> uint8_t { return (f > 0); });
        data_vec = pmt::init_u8vector(sym.size(), sym.data());
    } else {
        data_vec = pmt::init_f32vector(symbols.size(), symbols.data());
    }

    // ship it!
    metadata = pmt::dict_delete(metadata, PMTCONSTSTR__sample_rate());
    metadata = pmt::dict_add(
        metadata, PMTCONSTSTR__symbol_rate(), pmt::from_float(symbol_freq * samp_rate));

    message_port_pub(PMTCONSTSTR__pdu_out(), pmt::cons(metadata, data_vec));

    return;
} // end pdu_handler


/**
 * finds zero crossings in the data. Returns a vector of indexes
 * where zero crossings happen
 *
 * @param data - pointer to real waveform
 * @param len - length of data
 * @return std::vector<float> - list of zero crossing points
 */
std::vector<float> pdu_clock_recovery_impl::findZeroCrossings(const float* data,
                                                              const int len)
{
    std::vector<float> ans;
    float mid = 0; //= midpoint( data, len );

    ans.reserve(len / 2); // make a generous estimate of how many zero crossings there are
    for (int i = 1; i < len; i++) {
        if ((data[i] > mid) != (data[i - 1] > mid)) {
            // quick linear interpolation of zero crossing point, unit = indexs
            ans.push_back(i - 1 + (data[i - 1] - mid) / (data[i - 1] - data[i]));
        }
    } // end for(i

    return ans;
} // end findZeroCrossings

/**
 * Does a bunch  of pre-condition checks on the input PDU
 *
 * @param pdu - incoming PDU
 * @return bool - true if pre-conditions met
 */
bool pdu_clock_recovery_impl::inputCheck(pmt::pmt_t pdu)
{

    if (!pmt::is_pair(pdu)) {
        d_logger->warn("PDU is not a pair, dropping\n");
        return false;
    }

    pmt::pmt_t metadata = pmt::car(pdu);
    pmt::pmt_t pdu_data = pmt::cdr(pdu);

    if (!pmt::is_f32vector(pdu_data)) {
        d_logger->warn("PDU is not f32vector, dropping\n");
        return false;
    }

    if (!pmt::is_dict(metadata)) {
        d_logger->warn("PDU metadata is not dict, dropping\n");
        return false;
    }

    pmt::pmt_t pmt_samp_rate =
        pmt::dict_ref(metadata, PMTCONSTSTR__sample_rate(), pmt::get_PMT_NIL());
    if (pmt_samp_rate == pmt::get_PMT_NIL() || !pmt::is_number(pmt_samp_rate)) {
        d_logger->warn("no sample rate, dropping\n");
        return false;
    }

    return true;
} // end inputCheck

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
 */
void pdu_clock_recovery_impl::genSincWaveform(std::vector<float> crossings,
                                              const int sampLen,
                                              float* out,
                                              const int len)
{

    // drop sinc pulses at every zero crossing location
    for (const float& crossing : crossings) {
        int idx = round(crossing);

        if (0 < idx && idx < len) {
            if (idx == crossing) {
                out[idx] = 1.0f;
            } else if (idx < (len - 1)) {
                // this assumes d_lanczos_a == 1
                out[idx - 1] += fast_sinc(idx - 1 - crossing);
                out[idx] += fast_sinc(idx - crossing);
                out[idx + 1] += fast_sinc(idx + 1 - crossing);
            }

        } // end if( inbounds

    } // end for(i

    return;
} // end genSyncWaveform


/**
 * Finds the max fundamental freq present in the FFT magnitude array.
 *
 * @param mags - FFT magnitude
 * @param len - length of mags
 * @return int - index of mags representing the max fundamental freq
 */
int pdu_clock_recovery_impl::findMaxFundamental(const float* mags, const int len)
{
    float max_val = -1;
    int max_bin = 0;

    // first, calc DC reject
    int first_bin = std::ceil(d_dc_reject * len);


    // find max value across whole array
    for (int i = first_bin; i < len; i++) {
        if (d_mags[i] > max_val) {
            max_val = d_mags[i];
            max_bin = i;
        }
    }


    // make sure the largest magnitude wasn't a harmonic of the frequency we want
    float thresh = 0.5 * d_mags[max_bin];

    if (d_debug) {
        d_logger->debug("first pass max_bin {}({:e})   first_bin {}",max_bin,
                         d_mags[max_bin], first_bin);
        d_logger->debug("harmonic thresh {:e}", thresh);
    }


    // 'denom' is the harmonic we are checking (if denom is 5, we're checking if max_bin
    // is a 5th harmonic)
    // TODO WHY does this only process up to the 5th harmonic
    // TODO WHY does this start processing at 5th and go down.
    for (int denom = (SPS_MAX / 2); denom >= 2; denom--) {
        // calculate the index of the fundamental
        int funIdx = round(1.0f / (float)denom * max_bin);

        if (first_bin <= funIdx && funIdx < len) // avoid DC reject & inbounds
        {
            if (d_debug) {
                d_logger->debug("harmonic check {}({:e}) thresh {:e}  denom {}",
                                 funIdx, d_mags[funIdx], thresh, denom);
            }

            if (d_mags[funIdx] >= thresh || d_mags[funIdx - 1] >= thresh ||
                d_mags[funIdx + 1] >= thresh) {
                // max search across [-1 .. +1] region
                max_bin = funIdx - 1;
                max_val = d_mags[max_bin];
                for (int i = max_bin + 1; i <= max_bin + 2; i++) {
                    if (d_mags[i] > max_val) {
                        max_val = d_mags[i];
                        max_bin = i;
                    }
                }

                if (d_debug) {
                    d_logger->debug("harmonic found, denom {}, bin {}({:e})",
                                     denom, max_bin, d_mags[max_bin]);
                }

                break;

            } // end if( fundamental threshold

        } // end if( avoid DC reject

    } // end for(denom

    if (d_debug) {
        d_logger->debug("second pass max_bin {}({:e})", max_bin, 
                         d_mags[max_bin]);
    }

    return max_bin;
    ;
} // end findMaxFundamental

/**
 * Interpolates peak bin position
 *
 * @param mags - FFT magnitude
 * @param len - length of mags
 * @param max_bin - index into mags with max value
 * @return float - calculated peak bin
 */
float pdu_clock_recovery_impl::calcPeakBin(const float* mags,
                                           const int len,
                                           const int max_bin)
{
    float ans = max_bin;

    if ((max_bin > 0) and (max_bin < len - 1)) {
        // https://ccrma.stanford.edu/~jos/sasp/Quadratic_Interpolation_Spectral_Peaks.html
        float alpha = log(d_mags[max_bin - 1]);
        float beta = log(d_mags[max_bin]);
        float gamma = log(d_mags[max_bin + 1]);

        float bin_offset = 0.5f * (alpha - gamma) / (alpha - 2 * beta + gamma);
        ans = max_bin + bin_offset;

        if (std::isnan(ans) || std::isinf(ans) || (ans < 0)) {
            if (d_debug) {
                d_logger->debug("burstID:{} peak_bin is inf or nan, reverting",
                        d_burst_id);
                d_logger->debug("burstID:{} max_bin:{} alpha:{:e} beta:{:e} gamma:{:e}",
                        d_burst_id, max_bin, alpha, beta, gamma);
            }
            ans = max_bin;
        }

        if ((bin_offset > .5f) || (bin_offset < -.5f)) {
            // sanity check - an answer out of range\can occur if the max_bin is
            // not a local maxima, i.e. a linear increasing/decreasing set of points

            if (d_debug) {
                d_logger->debug("burstID:{} bin_offset outside sanity check {:e}, reverting",
                        d_burst_id, bin_offset);
            }

            ans = max_bin;
        }
    }

    return ans;
} // end calcPeakBin

/**
 * Calculate Peak phase
 *
 * @param fft - complex FFT data
 * @param fftsize - size of fft
 * @param max_bin - index of max bin
 * @param peak_bin - calculated peak bin
 * @return float - phase
 */
float pdu_clock_recovery_impl::calcPeakPhase(gr_complex* fft,
                                             const int fftsize,
                                             const int max_bin,
                                             const float peak_bin)
{
    float ans = 0;


    // interpolate real and imaginary peaks separately to get accurate phase
    int n_pts_to_interp = std::min(max_bin, 5);
    int maxi = std::min(max_bin + n_pts_to_interp, fftsize - 1);
    float real_peak = 0;
    float imag_peak = 0;

    for (int i = max_bin - n_pts_to_interp; i <= maxi; i++) {
        float sinc;
        if (i - peak_bin == 0) {
            sinc = 1;
        } else {
            sinc = std::sin(M_PI * (i - peak_bin)) / (M_PI * (i - peak_bin));
        }
        real_peak += std::real(fft[i]) * sinc;
        imag_peak += std::imag(fft[i]) * sinc;
    } // end for(i

    ans = std::arg(gr_complex(real_peak, imag_peak));

    return ans;
} // end calcPeakPhase


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
std::vector<float> pdu_clock_recovery_impl::extractSymbols(const float* data,
                                                           const int len,
                                                           const float symbol_freq,
                                                           const float phase,
                                                           const int offset)
{
    std::vector<float> ans;

    ans.reserve((int)len * symbol_freq + 100);

    float clock_phase = phase / (2 * M_PI);
    clock_phase += 0.5f; // we want sample times, not zero crossing times
    if (clock_phase <= 0) {
        clock_phase += 1.0f;
    }
    clock_phase += symbol_freq;

    // NOTE: The SincWaveform & FFT were not taken from the start of the file. We need to
    // offset clock_phase to account for this
    if (d_debug) {
        d_logger->debug("Pre correction phase {:e}   clock_phase {:e}",phase,
                         clock_phase);
    }
    clock_phase -= offset * symbol_freq;
    while (clock_phase <= 0) {
        clock_phase += 1.0f;
    }
    while (clock_phase > 1) {
        clock_phase -= 1.0f;
    }
    if (d_debug) {
        d_logger->debug("phase {:e}   clock_phase {:e}", phase, clock_phase);
    }


    for (int i = 1; i < len; i++) {
        if (clock_phase >= 1) {
            clock_phase -= 1.0f;
            float mu = clock_phase / symbol_freq;
            float interp = mu * data[i - 1] + (1 - mu) * data[i];
            ans.push_back(interp);
        }
        clock_phase += symbol_freq;
    }

    return ans;
} // end extractSymbols

} /* namespace pdu_utils */
} /* namespace gr */
