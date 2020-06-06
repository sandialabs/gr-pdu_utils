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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <volk/volk.h>
#include <gnuradio/io_signature.h>
#include "pdu_pfb_resamp_cc_impl.h"
#include "complex"

namespace gr {
  namespace pdu_utils {

    pdu_pfb_resamp_cc::sptr
    pdu_pfb_resamp_cc::make(const std::vector<float> taps, int n_filters, float resamp_rate)
    {
      return gnuradio::get_initial_sptr
        (new pdu_pfb_resamp_cc_impl(taps, n_filters, resamp_rate));
    }

    /*
     * The private constructor
     */
    pdu_pfb_resamp_cc_impl::pdu_pfb_resamp_cc_impl(const std::vector<float> taps, int n_filters, float resamp_rate)
      : gr::block("pdu_pfb_resamp_ff",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0)),
        d_taps(taps),
        d_nfilts(n_filters),
        d_resamp_rate(resamp_rate)
    {
      d_pfb = new filter::kernel::pfb_arb_resampler_ccf(1, d_taps, d_nfilts);
      d_pfb->set_rate(d_resamp_rate);

      d_in = NULL;
      d_out = NULL;
      d_input_size = 0;

      std::cout << "started pdu pfb resampler with " << d_taps.size() << " taps" << std::endl;
      message_port_register_in(PMTCONSTSTR__PDU_IN);
      set_msg_handler(PMTCONSTSTR__PDU_IN,
          boost::bind(&pdu_pfb_resamp_cc_impl::handle_pdu, this, _1));
      message_port_register_out(PMTCONSTSTR__PDU_OUT);
    }

    /*
     * Our virtual destructor.
     */
    pdu_pfb_resamp_cc_impl::~pdu_pfb_resamp_cc_impl()
    {
      if (d_in != NULL) volk_free(d_in);
      if (d_out != NULL) volk_free(d_out);
    }

    void pdu_pfb_resamp_cc_impl::resize_arrays(size_t newSize) {
      if (d_in != NULL) volk_free(d_in);
      if (d_out != NULL) volk_free(d_out);
      d_in = (gr_complex*)volk_malloc(sizeof(gr_complex)*newSize, volk_get_alignment());
      d_out = (gr_complex*)volk_malloc((sizeof(gr_complex)*newSize*d_resamp_rate + 1), volk_get_alignment());
      d_input_size = newSize;
    }

    void
    pdu_pfb_resamp_cc_impl::handle_pdu(pmt::pmt_t pdu)
    {
      /* code */
      pmt::pmt_t meta = pmt::car(pdu);
      pmt::pmt_t v_data = pmt::cdr(pdu);
      if (pmt::is_c32vector(v_data))
      {
        uint32_t v_len = pmt::length(v_data);
        size_t v_itemsize = pmt::uniform_vector_itemsize(v_data);
        //std::cout << "got uniform vector of length " << v_len << " and itemsize " << v_itemsize << std::endl;

        const std::vector<std::complex<float> > input_data = pmt::c32vector_elements(v_data);
        // allocate enough space to pad with 1/2 filter length on each side of the data (group delay)
        // also tack on some extra memory because the in-tree filter() call accesses out-of-bounds memory
        if (d_input_size < input_data.size() + d_nfilts*2) {
          // Give ourselves some extra size to prevent a ton of reallocs
          resize_arrays(input_data.size()*1.25 + 2*d_nfilts);
        }
        //std::vector<std::complex<float> > d_out, in_data; // output
        const float z=0.0;
        int start = d_nfilts / 2;
        int num_read;

        // adjust length by filter taps
        for (int ii=0; ii < start; ii++) d_in[ii] = input_data[0];
        memcpy(&d_in[start], &input_data[0], sizeof(gr_complex)*input_data.size());
        for (int ii=0; ii < 2*start; ii++) d_in[ii+start+input_data.size()] = input_data[input_data.size()-1];

        int n_out = d_pfb->filter(d_out, d_in, input_data.size() + start, num_read);  // C++11??

        //in_data.reserve(2*start + d_in.size());
        //in_data.clear();
        //for (int ii=0; ii < start; ii++) in_data.insert(in_data.end(), d_in[0]);
        //in_data.insert(in_data.end(), d_in.begin(), d_in.end());
        //for (int ii=0; ii < start; ii++) in_data.insert(in_data.end(), d_in.back());

        //uint32_t out_len = int(in_data.size() * d_resamp_rate) + 10;
        //d_out.resize(out_len);

        //int n_out = d_pfb->filter(d_out.data(), in_data.data(), in_data.size(), num_read);  // C++11??
        //std::cout << "input: " << in_data.data() << ", output: " << d_out.data() << std::endl;

        //std::cout << "\tv_len = " << v_len << ", pfb outlen = " << n_out << ", num_read "  << num_read << std::endl;
        //std::cout << "\tFILTERING for " << v_len << " output samples, ("  << in_data.size() << " in, " << d_out.size() << " out)" << std::endl;
        if (pmt::dict_has_key(meta, pmt::mp("sample_rate"))) {
          double sample_rate = pmt::to_double(pmt::dict_ref(meta, pmt::mp("sample_rate"), pmt::PMT_NIL));
          sample_rate *= d_resamp_rate;
          meta = pmt::dict_delete(meta, pmt::mp("sample_rate"));
          meta = pmt::dict_add(meta, pmt::mp("sample_rate"), pmt::from_double(sample_rate));
        }

        message_port_pub(PMTCONSTSTR__PDU_OUT, (pmt::cons(meta, pmt::init_c32vector(n_out, d_out) )));
        //message_port_pub(PMTCONSTSTR__PDU_OUT, (pmt::cons(meta, pmt::init_c32vector(int(n_out-(start * d_resamp_rate)), (const std::complex<float> *)&d_out[0]) )));
        //message_port_pub(PMTCONSTSTR__PDU_OUT, (pmt::cons(meta, pmt::init_f32vector(in_data.size(), (const float *)&in_data[0]) )));

      } else {
        std::cout << "WARNING: got unknown vector type from PDU, dropping" << std::endl;

      }


    }

  } /* namespace pdu_utils */
} /* namespace gr */
