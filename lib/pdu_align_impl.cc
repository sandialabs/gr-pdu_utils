/* -*- c++ -*- */
/*
 * Copyright 2018, 2019, 2020 National Technology & Engineering Solutions of Sandia, LLC
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
#include <pdu_utils/constants.h>
#include <volk/volk.h>

namespace gr {
namespace pdu_utils {

pdu_align::sptr
pdu_align::make(std::string syncwords, int threshold, int offset, align_modes mode)
{
    return gnuradio::get_initial_sptr(
        new pdu_align_impl(syncwords, threshold, offset, mode));
}

/*
 * The private constructor
 */
pdu_align_impl::pdu_align_impl(std::string syncwords,
                               int threshold,
                               int offset,
                               align_modes mode)
    : gr::block(
          "pdu_align", gr::io_signature::make(0, 0, 0), gr::io_signature::make(0, 0, 0)),
      d_threshold(threshold),
      d_offset(offset),
      d_mode(mode)
{

    // convert comma delimited binary syncwords to vector of unsigned ints
    std::stringstream ss(syncwords);
    while (ss.good()) {
        std::string syncword;
        getline(ss, syncword, ',');
        // remove leading whitespace
        while (std::isspace(syncword[0])) {
            syncword.erase(syncword.begin());
        }
        // remove '0b' prefix if it's there
        if (syncword.length() > 2 && syncword[0] == '0' && syncword[1] == 'b') {
            syncword = syncword.substr(2, std::string::npos);
        }
        size_t syncword_len;
        uint64_t syncword_int, mask_int;
        try {
            syncword_int = std::stoul(syncword, &syncword_len, 2);
            mask_int = (syncword_len >= 64 ? -1 : (1lu << syncword_len) - 1);
        } catch (std::invalid_argument ex) {
            GR_LOG_ERROR(d_logger,
                         boost::format("unable to parse syncword '%s' (must be base 2)") %
                             syncword.c_str());
            exit(1);
        } catch (std::out_of_range ex) {

            GR_LOG_ERROR(d_logger,
                         boost::format("syncword '%s' out of range (max of 64 bits)") %
                             syncword.c_str());
            exit(1);
        }
        GR_LOG_DEBUG(d_logger,
                     boost::format("PDU align syncword: 0x%016lX (mask 0x%016lX)") %
                         syncword_int % mask_int);
        d_syncwords.push_back(syncword_int);
        d_syncword_lens.push_back(syncword_len);
        d_masks.push_back(mask_int);
    }


    message_port_register_in(PMTCONSTSTR__PDU_IN);
    message_port_register_out(PMTCONSTSTR__PDU_OUT);
    set_msg_handler(PMTCONSTSTR__PDU_IN,
                    boost::bind(&pdu_align_impl::pdu_handler, this, _1));
}

/*
 * Our virtual destructor.
 */
pdu_align_impl::~pdu_align_impl() {}

void pdu_align_impl::update_time_metadata(pmt::pmt_t& metadata, int start_idx)
{
    // if the pmt has a start_time or duration field to udpate
    if (pmt::dict_has_key(metadata, PMTCONSTSTR__START_TIME) ||
        pmt::dict_has_key(metadata, PMTCONSTSTR__DURATION)) {
        // get the sample rate (or symbol rate)
        float samp_rate;

        if (pmt::dict_has_key(metadata, PMTCONSTSTR__SAMP_RATE)) {
            samp_rate = pmt::to_float(
                pmt::dict_ref(metadata, PMTCONSTSTR__SAMP_RATE, pmt::PMT_NIL));
        } else if (pmt::dict_has_key(metadata, PMTCONSTSTR__SYM_RATE)) {
            samp_rate = pmt::to_float(
                pmt::dict_ref(metadata, PMTCONSTSTR__SYM_RATE, pmt::PMT_NIL));
        } else {
            return;
        }
        // update the fields
        float offset = start_idx / samp_rate;

        if (pmt::dict_has_key(metadata, PMTCONSTSTR__START_TIME)) {
            double start_time = pmt::to_double(
                pmt::dict_ref(metadata, PMTCONSTSTR__START_TIME, pmt::PMT_NIL));
            metadata = pmt::dict_delete(metadata, PMTCONSTSTR__START_TIME);
            metadata = pmt::dict_add(
                metadata, PMTCONSTSTR__START_TIME, pmt::from_double(start_time + offset));
            metadata = pmt::dict_add(
                metadata, PMTCONSTSTR__START_TIME_OFFSET, pmt::from_double(offset));
            // printf("incremented start time by %f\n", offset);
        }

        if (pmt::dict_has_key(metadata, PMTCONSTSTR__DURATION)) {
            float duration = pmt::to_float(
                pmt::dict_ref(metadata, PMTCONSTSTR__DURATION, pmt::PMT_NIL));
            metadata = pmt::dict_delete(metadata, PMTCONSTSTR__DURATION);
            metadata = pmt::dict_add(
                metadata, PMTCONSTSTR__DURATION, pmt::from_float(duration - offset));
            // printf("decreased duration by %f\n", offset);
        }
    }
}

void pdu_align_impl::pdu_handler(pmt::pmt_t pdu)
{
    if (!pmt::is_pair(pdu)) {
        GR_LOG_DEBUG(d_logger, "WARNING: PDU is not a pair, dropping");
        return;
    }

    pmt::pmt_t metadata = pmt::car(pdu);
    pmt::pmt_t pdu_data = pmt::cdr(pdu);

    if (!pmt::is_u8vector(pdu_data)) {
        GR_LOG_DEBUG(d_logger, "WARNING: PDU not u8vector, dropping");
        return;
    }

    size_t data_len;
    const uint8_t* data = pmt::u8vector_elements(pdu_data, data_len);

    uint64_t nwrong = 0;
    uint64_t data_reg = 0;

    // take the bits one at a time, checking for a match
    for (int bit_idx = 0; bit_idx < data_len; bit_idx++) {

        // shift in the next bit
        data_reg = (data_reg << 1) | (data[bit_idx] & 0x1);

        // compare with each syncword
        for (int sync_idx = 0; sync_idx < d_syncwords.size(); sync_idx++) {
            uint64_t to_test = (data_reg ^ d_syncwords[sync_idx]) & d_masks[sync_idx];
            // printf("to test: %016lx vs %016lx -> %016lx\n", data_reg &
            // d_masks[sync_idx],
            //   d_syncwords[sync_idx] & d_masks[sync_idx],to_test);
            volk_64u_popcnt(&nwrong, to_test);
            // printf("nwrong = %ld,bit_idx = %d, len = %ld,threshold = %d\n",nwrong,
            //   bit_idx,d_syncword_lens[sync_idx],d_threshold);

            // check for a match
            if ((nwrong <= d_threshold) and
                ((size_t)bit_idx >= (d_syncword_lens[sync_idx] - 1))) {
                int start_idx = bit_idx + 1 + d_offset;
                // printf("found match at index %d\n", start_idx);
                update_time_metadata(metadata, start_idx);
                pmt::pmt_t data_vec =
                    pmt::init_u8vector(data_len - start_idx, data + start_idx);
                message_port_pub(PMTCONSTSTR__PDU_OUT, pmt::cons(metadata, data_vec));
                return;
            }
        }
    }

    // syncword not found - what should we do?
    if (d_mode == ALIGN_FORWARD) {
        message_port_pub(PMTCONSTSTR__PDU_OUT, pdu);
    } else if (d_mode == ALIGN_EMPTY) {
        message_port_pub(PMTCONSTSTR__PDU_OUT,
                         pmt::cons(metadata, pmt::init_u8vector(0, {})));
    }


    // GR_LOG_DEBUG(d_logger, "Syncword not found, dropping PDU");
}

} /* namespace pdu_utils */
} /* namespace gr */
