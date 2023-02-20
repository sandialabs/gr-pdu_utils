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

#include "pdu_align_impl.h"
#include <gnuradio/io_signature.h>
#include <gnuradio/pdu_utils/constants.h>
#include <volk/volk.h>
#include <boost/format.hpp>

namespace gr {
namespace pdu_utils {

pdu_align::sptr
pdu_align::make(std::string syncwords, int threshold, int offset, align_modes mode, align_match_mode match_mode)
{
    return gnuradio::make_block_sptr<pdu_align_impl>(syncwords, threshold, offset, mode, match_mode);
}

/*
 * The private constructor
 */
pdu_align_impl::pdu_align_impl(std::string syncwords,
                               int threshold,
                               int offset,
                               align_modes mode,
			       align_match_mode match_mode)
    : gr::block(
          "pdu_align", gr::io_signature::make(0, 0, 0), gr::io_signature::make(0, 0, 0)),
      d_threshold(threshold),
      d_offset(offset),
      d_mode(mode),
      d_match_mode(match_mode)
{

    // convert comma delimited binary syncwords to vector of unsigned ints
    std::stringstream ss(syncwords);
    
    while (ss.good()) {
	bool is_hex = false;
        std::string syncword;
        getline(ss, syncword, ',');
        // remove leading whitespace
        while (std::isspace(syncword[0])) {
            syncword.erase(syncword.begin());
        }
        // check for hex prefix
	if (syncword.length() > 2 && syncword[0] == '0' && syncword[1] == 'x'){
	    is_hex = true;
	}
	// remove '0b' prefix if it's there
        if (syncword.length() > 2 && syncword[0] == '0' && syncword[1] == 'b') {
            syncword = syncword.substr(2, std::string::npos);
        }
        size_t syncword_len;
        uint64_t syncword_int, mask_int;
        try {
            syncword_int = (is_hex) ? std::stoul(syncword, &syncword_len, 16)
		    		    : std::stoul(syncword, &syncword_len, 2);
	    syncword_len = (is_hex) ? 4 * (syncword_len - 2)
		    	            : syncword_len;
            mask_int = (syncword_len >= 64 ? -1 : (1lu << syncword_len) - 1);
            std::cout << "syncword_int = " << std::bitset<16>(syncword_int) << std::endl;
	} catch (std::invalid_argument& ex) {
            d_logger->error("unable to parse syncword '{}' (must be base 2 or 16)",
                             syncword.c_str());
            exit(1);
        } catch (std::out_of_range& ex) {

            d_logger->error("syncword '{}' out of range (max of 64 bits)",
                             syncword.c_str());
            exit(1);
        }
        d_logger->debug("PDU align syncword: {:#+16X} (mask {:#+16X})",
                         syncword_int, mask_int);
        d_syncwords.push_back(syncword_int);
        d_syncword_lens.push_back(syncword_len);
        d_masks.push_back(mask_int);
	std::cout << "syncword = " << d_syncwords[0] << std::endl;
	std::cout << "syncword len = " << d_syncword_lens[0] << std::endl;
    }


    message_port_register_in(PMTCONSTSTR__pdu_in());
    message_port_register_out(PMTCONSTSTR__pdu_out());
    set_msg_handler(PMTCONSTSTR__pdu_in(),
                    [this](pmt::pmt_t msg) { this->pdu_handler(msg); });
}

/*
 * Our virtual destructor.
 */
pdu_align_impl::~pdu_align_impl() {}

void pdu_align_impl::update_time_metadata(pmt::pmt_t& metadata, int start_idx)
{
    // if the pmt has a start_time or duration field to udpate
    if (pmt::dict_has_key(metadata, PMTCONSTSTR__start_time()) ||
        pmt::dict_has_key(metadata, PMTCONSTSTR__duration())) {
        // get the sample rate (or symbol rate)
        float samp_rate;

        if (pmt::dict_has_key(metadata, PMTCONSTSTR__sample_rate())) {
            samp_rate = pmt::to_float(
                pmt::dict_ref(metadata, PMTCONSTSTR__sample_rate(), pmt::PMT_NIL));
        } else if (pmt::dict_has_key(metadata, PMTCONSTSTR__symbol_rate())) {
            samp_rate = pmt::to_float(
                pmt::dict_ref(metadata, PMTCONSTSTR__symbol_rate(), pmt::PMT_NIL));
        } else {
            return;
        }
        // update the fields
        float offset = start_idx / samp_rate;

        if (pmt::dict_has_key(metadata, PMTCONSTSTR__start_time())) {
            double start_time = pmt::to_double(
                pmt::dict_ref(metadata, PMTCONSTSTR__start_time(), pmt::PMT_NIL));
            metadata = pmt::dict_delete(metadata, PMTCONSTSTR__start_time());
            metadata = pmt::dict_add(metadata,
                                     PMTCONSTSTR__start_time(),
                                     pmt::from_double(start_time + offset));
            metadata = pmt::dict_add(
                metadata, PMTCONSTSTR__start_time_offset(), pmt::from_double(offset));
            // printf("incremented start time by %f\n", offset);
        }

        if (pmt::dict_has_key(metadata, PMTCONSTSTR__duration())) {
            float duration = pmt::to_float(
                pmt::dict_ref(metadata, PMTCONSTSTR__duration(), pmt::PMT_NIL));
            metadata = pmt::dict_delete(metadata, PMTCONSTSTR__duration());
            metadata = pmt::dict_add(
                metadata, PMTCONSTSTR__duration(), pmt::from_float(duration - offset));
            // printf("decreased duration by %f\n", offset);
        }
    }
}

void pdu_align_impl::pdu_handler(pmt::pmt_t pdu)
{
    if (!pmt::is_pair(pdu)) {
        d_logger->debug("WARNING: PDU is not a pair, dropping");
        return;
    }

    pmt::pmt_t metadata = pmt::car(pdu);
    pmt::pmt_t pdu_data = pmt::cdr(pdu);

    if (!pmt::is_u8vector(pdu_data)) {
        d_logger->debug("WARNING: PDU not u8vector, dropping");
        return;
    }

    size_t data_len;
    const uint8_t* data = pmt::u8vector_elements(pdu_data, data_len);

    uint64_t nwrong = 0;
    uint64_t data_reg = 0;
    int current_best = d_threshold + 1;
    bool found = false;
    int look_ahead = d_syncword_lens[0]/2;
    int start_idx = 0;

    // take the bits one at a time, checking for a match
    for (size_t bit_idx = 0; bit_idx < data_len; bit_idx++) {
	  
	// shift in the next bit
        data_reg = (data_reg << 1) | (data[bit_idx] & 0x1);
        
	// compare with each syncword
        for (size_t sync_idx = 0; sync_idx < d_syncwords.size(); sync_idx++) {
            uint64_t to_test = (data_reg ^ d_syncwords[sync_idx]) & d_masks[sync_idx];
            // printf("to test: %016lx vs %016lx -> %016lx\n", data_reg &
            // d_masks[sync_idx],
            //   d_syncwords[sync_idx] & d_masks[sync_idx],to_test);
            volk_64u_popcnt(&nwrong, to_test);
            // printf("nwrong = %ld,bit_idx = %d, len = %ld,threshold = %d\n",nwrong,
            //   bit_idx,d_syncword_lens[sync_idx],d_threshold);
	
            // check for a match
            if (((int)nwrong <= d_threshold) and
                ((size_t)bit_idx >= (d_syncword_lens[sync_idx] - 1)) and
	        ((int)nwrong < current_best))  {
                found = true;
		current_best = nwrong;
		start_idx = bit_idx + 1 + d_offset;
                look_ahead = d_syncword_lens[sync_idx]/2;
		//printf("found match at index %d\n", start_idx);
                
		if ((d_match_mode == ALIGN_FIRST_MATCH) or (current_best == 0)){
	            update_time_metadata(metadata, start_idx);
                    pmt::pmt_t data_vec =
                    pmt::init_u8vector(data_len - start_idx, data + start_idx);
               	     message_port_pub(PMTCONSTSTR__pdu_out(), pmt::cons(metadata, data_vec));
                    return;
		}
	    }
	}
	   if (d_match_mode == ALIGN_BEST_MATCH){
                if (found and (int)bit_idx > (start_idx + look_ahead)){
                      update_time_metadata(metadata, start_idx);
                      pmt::pmt_t data_vec =
                      pmt::init_u8vector(data_len - start_idx, data + start_idx);
                      message_port_pub(PMTCONSTSTR__pdu_out(), pmt::cons(metadata, data_vec));
                      return;

                        }
                }
        

    }
	// ALIGN_FIRST_MATCH or ALIGN_BEST_MATCH
	// d_match_mode
	//
    // If match found but didn't publish yet, publish now
    if (found){
	    update_time_metadata(metadata, start_idx);
            pmt::pmt_t data_vec =
            pmt::init_u8vector(data_len - start_idx, data + start_idx);
            message_port_pub(PMTCONSTSTR__pdu_out(), pmt::cons(metadata, data_vec));
	    return;
    }
    // syncword not found - what should we do?
    if (d_mode == ALIGN_FORWARD) {
        message_port_pub(PMTCONSTSTR__pdu_out(), pdu);
    } else if (d_mode == ALIGN_EMPTY) {
        message_port_pub(PMTCONSTSTR__pdu_out(),
                         pmt::cons(metadata, pmt::init_u8vector(0, {})));
    }


    // d_logger->debug("Syncword not found, dropping PDU");
}

} /* namespace pdu_utils */
} /* namespace gr */
