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
#include "pdu_align_impl.h"

namespace gr {
  namespace pdu_utils {

    pdu_align::sptr
    pdu_align::make(std::string syncwords, int threshold, int offset)
    {
      return gnuradio::get_initial_sptr
        (new pdu_align_impl(syncwords, threshold, offset));
    }

    /*
     * The private constructor
     */
    pdu_align_impl::pdu_align_impl(std::string syncwords, int threshold, int offset)
      : gr::block("pdu_align",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0)),
      d_threshold(threshold), d_offset(offset)
    {

      // convert comma delimited binary syncwords to vector of unsigned ints
      std::stringstream ss(syncwords);
      while (ss.good()) {
        std::string syncword;
        getline(ss, syncword, ',');
        // remove leading whitespace
        while (std::isspace(syncword[0])) { syncword.erase(syncword.begin()); }
        // remove '0b' prefix if it's there
        if (syncword.length() > 2 && syncword[0] == '0' && syncword[1] == 'b') {
          syncword = syncword.substr(2, std::string::npos);
        }
        size_t syncword_len;
        uint64_t syncword_int, mask_int;
        try {
          syncword_int = std::stoul(syncword, &syncword_len, 2);
          mask_int = (syncword_len >= 64 ? -1 : (1lu << syncword_len) - 1);
        } catch(std::invalid_argument ex) {
          printf("unable to parse syncword '%s' (must be base 2)\n", syncword.c_str());
          exit(1);
        } catch (std::out_of_range ex) {
          printf("syncword '%s' out of range (max of 64 bits)\n", syncword.c_str());
          exit(1);
        }
        printf("PDU align syncword: 0x%016lX (mask 0x%016lX)\n", syncword_int, mask_int);
        d_syncwords.push_back(syncword_int);
        d_masks.push_back(mask_int);
      }

      message_port_register_in(pmt::mp("pdu_in"));
      message_port_register_out(pmt::mp("pdu_out"));
      set_msg_handler(pmt::mp("pdu_in"), boost::bind(&pdu_align_impl::pdu_handler, this, _1));
    }

    /*
     * Our virtual destructor.
     */
    pdu_align_impl::~pdu_align_impl()
    {
    }

    void pdu_align_impl::update_time_metadata(pmt::pmt_t &metadata, int start_idx) {
      // if the pmt has a start_time or duration field to udpate
      if (pmt::dict_has_key(metadata, pmt::mp("start_time")) || pmt::dict_has_key(metadata, pmt::mp("duration"))) {
        // get the sample rate (or symbol rate)
        float samp_rate;
        if (pmt::dict_has_key(metadata, pmt::mp("sample_rate"))) {
          samp_rate = pmt::to_float(pmt::dict_ref(metadata, pmt::mp("sample_rate"), pmt::PMT_NIL));
        } else if (pmt::dict_has_key(metadata, pmt::mp("symbol_rate"))) {
          samp_rate = pmt::to_float(pmt::dict_ref(metadata, pmt::mp("symbol_rate"), pmt::PMT_NIL));
        } else {
          return;
        }
        // update the fields
        float offset = start_idx / samp_rate;
        if (pmt::dict_has_key(metadata, pmt::mp("start_time"))) {
          double start_time = pmt::to_double(pmt::dict_ref(metadata, pmt::mp("start_time"), pmt::PMT_NIL));
          metadata = pmt::dict_delete(metadata, pmt::mp("start_time"));
          metadata = pmt::dict_add(metadata, pmt::mp("start_time"), pmt::from_double(start_time + offset));
          metadata = pmt::dict_add(metadata, pmt::mp("start_time_offset"), pmt::from_double(offset));
          //printf("incremented start time by %f\n", offset);
        }
        if (pmt::dict_has_key(metadata, pmt::mp("duration"))) {
          float duration = pmt::to_float(pmt::dict_ref(metadata, pmt::mp("duration"), pmt::PMT_NIL));
          metadata = pmt::dict_delete(metadata, pmt::mp("duration"));
          metadata = pmt::dict_add(metadata, pmt::mp("duration"), pmt::from_float(duration - offset));
          //printf("decreased duration by %f\n", offset);
        }
      }
    }

    void pdu_align_impl::pdu_handler(pmt::pmt_t pdu) {
      if (!pmt::is_pair(pdu)) {
        printf("WARNING: PDU is not a pair, dropping\n");
        return;
      }

      pmt::pmt_t metadata = pmt::car(pdu);
      pmt::pmt_t pdu_data = pmt::cdr(pdu);

      if (!pmt::is_u8vector(pdu_data)) {
        printf("WARNING: PDU not u8vector, dropping\n");
        return;
      }

      size_t data_len;
      const uint8_t* data = pmt::u8vector_elements(pdu_data, data_len);

      uint64_t nwrong = 0;
      uint64_t data_reg = 0;

      // take the bits one at a time, checking for a match
      for (int bit_idx=0; bit_idx<data_len; bit_idx++) {

        // shift in the next bit
        data_reg = (data_reg << 1) | (data[bit_idx] & 0x1);

        // compare with each syncword
        for (int sync_idx=0; sync_idx<d_syncwords.size(); sync_idx++) {
          uint64_t to_test = (data_reg ^ d_syncwords[sync_idx]) & d_masks[sync_idx];
          //printf("to test: %016lx vs %016lx\n", data_reg & d_masks[sync_idx], d_syncwords[sync_idx] & d_masks[sync_idx]);
          volk_64u_popcnt(&nwrong, to_test);

          // check for a match
          if (nwrong <= d_threshold) {
            int start_idx = bit_idx + 1 + d_offset;
            //printf("found match at index %d\n", start_idx);
            update_time_metadata(metadata, start_idx);
            pmt::pmt_t data_vec = pmt::init_u8vector(data_len - start_idx, data + start_idx);
            message_port_pub(pmt::mp("pdu_out"), pmt::cons(metadata, data_vec));
            return;
          }
        }
      }

      //GR_LOG_DEBUG(d_logger, "Syncword not found, dropping PDU");
    }

  } /* namespace pdu_utils */
} /* namespace gr */

