/* -*- c++ -*- */
/*
 * Copyright 2018, 2019, 2020, 2022 National Technology & Engineering Solutions of Sandia
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "access_code_to_pdu_impl.h"
#include <gnuradio/io_signature.h>
#include <gnuradio/pdu_utils/constants.h>
#include <volk/volk.h>
#include <bitset>
#include <boost/format.hpp>
namespace gr {
namespace pdu_utils {

access_code_to_pdu::sptr access_code_to_pdu::make(std::string access_code,
                                                  std::string tail_sync,
                                                  uint32_t burst_len,
                                                  uint32_t threshold,
                                                  sync_mode syncmode,
                                                  read_mode readmode)
{
    return gnuradio::make_block_sptr<access_code_to_pdu_impl>(
        access_code, tail_sync, burst_len, threshold, syncmode, readmode);
}


/*
 * The private constructor
 */
access_code_to_pdu_impl::access_code_to_pdu_impl(std::string access_code,
                                                 std::string tail_sync,
                                                 uint32_t burst_len,
                                                 uint32_t threshold,
                                                 sync_mode syncmode,
                                                 read_mode readmode)

    : gr::sync_block("access_code_to_pdu",
                     gr::io_signature::make(1, 1, sizeof(uint8_t)),
                     gr::io_signature::make(0, 0, 0)),
      d_data_reg(0),
      d_burst_counter(0),
      d_reversed(false),
      d_nread(0),
      d_bit_index(0),
      d_burst_len(burst_len),
      d_threshold(threshold),
      d_syncmode(syncmode),
      d_readmode(readmode),
      d_lock(false)
{
    // read in syncword strings and parse data
    set_sync(access_code, &d_access_code, &d_access_mask, &d_access_len);
    set_sync(tail_sync, &d_tail_sync, &d_tail_mask, &d_tail_len);

    // throw error if access code is empty and strict mode is not on
    if (d_access_len == 0 && d_readmode != READ_STRICT) {
        d_logger->error("access code should not be empty outside of strict mode");
        throw std::runtime_error("");
    }

    // make sure burst length is at least the size of the sum of the syncwords
    if (d_burst_len < d_access_len + d_tail_len) {
        d_logger->error("total burst length shorter than syncword(s)");
        throw std::runtime_error("");
    }

    // reserve memory for the output buffer
    d_output.reserve(sizeof(uint8_t) * d_burst_len);

    // set up PDU message output
    message_port_register_out(PMTCONSTSTR__pdu_out());
}

/*
 * Our virtual destructor.
 */
access_code_to_pdu_impl::~access_code_to_pdu_impl() {}

void access_code_to_pdu_impl::set_sync(const std::string sync_string,
                                       uint64_t* sync,
                                       uint64_t* mask,
                                       uint32_t* len)
{
    // convert binary or hexadecimal string to unsigned int
    // with a mask and syncword length
    std::stringstream ss(sync_string);
    bool is_hex = false;
    std::string syncword;
    getline(ss, syncword);
    if (syncword.empty()) {
        *sync = 0x0;
        *mask = 0x0;
        *len = 0x0;
        return;
    }
    // remove leading whitespace
    while (std::isspace(syncword[0])) {
        syncword.erase(syncword.begin());
    }
    // check for hex prefix
    if (syncword.length() > 2 && syncword[0] == '0' && syncword[1] == 'x') {
        is_hex = true;
    }
    // remove '0b' prefix if it's there
    if (syncword.length() > 2 && syncword[0] == '0' && syncword[1] == 'b') {
        syncword = syncword.substr(2, std::string::npos);
    }
    size_t syncword_len;
    uint64_t syncword_int, mask_int;
    // interpret string as unsigned integer, with accompanying length and mask
    try {
        syncword_int = (is_hex) ? std::stoul(syncword, &syncword_len, 16)
                                : std::stoul(syncword, &syncword_len, 2);
        syncword_len = (is_hex) ? 4 * (syncword_len - 2) : syncword_len;
        mask_int = (syncword_len >= 64 ? -1 : (1lu << syncword_len) - 1);
    } catch (std::invalid_argument& ex) {
        d_logger->error("unable to parse syncword '{}' (must be base 2 or 16)",
                syncword.c_str());
        throw std::runtime_error("");
    } catch (std::out_of_range& ex) {
        d_logger->error("syncword '{}' out of range (max of 64 bits)",
                         syncword.c_str());
        throw std::runtime_error("");
    }
    d_logger->debug("syncword: {:#+16X} (mask {:#+16X})",syncword_int,
                     mask_int);
    *sync = syncword_int;
    *mask = mask_int;
    *len = syncword_len;
}

// correlate for the access code, return true if the threshold is in range
// turn on flag if code was bit-reversed
bool access_code_to_pdu_impl::check_access_code()
{
    uint64_t wrong_bits;
    uint64_t nwrong;

    wrong_bits = (d_data_reg ^ d_access_code) & d_access_mask;
    volk_64u_popcnt(&nwrong, wrong_bits);

    if (nwrong <= d_threshold) {
        d_reversed = false;
        return true;
    } else if (nwrong >= d_access_len - d_threshold) {
        d_reversed = true;
        return true;
    }
    return false;
}

bool access_code_to_pdu_impl::check_tail_sync()
{
    // if the tail sync word is empty, return true
    if (!d_tail_len) {
        return true;
    }
    // return true if tail sync is within threshold
    // tail sync must be bit-reversed if access code was
    uint64_t wrong_bits = 0;
    uint64_t nwrong = d_threshold + 1;
    wrong_bits = (d_data_reg ^ d_tail_sync) & d_tail_mask;
    volk_64u_popcnt(&nwrong, wrong_bits);
    if (!d_reversed_vector[0] && nwrong <= d_threshold) {
        return true;
    } else if (d_reversed_vector[0] && nwrong >= d_tail_len - d_threshold) {
        return true;
    }
    return false;
}

void access_code_to_pdu_impl::publish_message()
{
    pmt::pmt_t meta_dict = pmt::make_dict();
    std::vector<uint8_t> output;

    // tag if the burst was bit-reversed
    if (d_reversed_vector[0]) {
        meta_dict =
            pmt::dict_add(meta_dict, PMTCONSTSTR__bit_reversed(), pmt::from_bool(true));
    } else {
        meta_dict =
            pmt::dict_add(meta_dict, PMTCONSTSTR__bit_reversed(), pmt::from_bool(false));
    }
    // add burst ID tag
    meta_dict = pmt::dict_add(
        meta_dict, PMTCONSTSTR__pdu_num(), pmt::from_uint64(d_burst_counter));
    // add tag of absolute bit index from beginning of stream
    meta_dict = pmt::dict_add(meta_dict,
                              PMTCONSTSTR__bit_index(),
                              pmt::from_uint64(d_bit_index + 1 - d_burst_len));
    if (d_reversed_vector[0]) {
        // bit-reverse PDU data if syncword was bit-reversed
        for (size_t i = 0; i < d_output.size(); i++) {
            output.push_back(d_output[i] ^ 0x1);
        }
    } else {
        output = d_output;
    }

    // remove syncwords from output PDU if discard setting is active
    switch (d_syncmode) {
      case SYNC_DISCARD:
        output.resize(output.size() - d_tail_len);
        output.erase(output.begin(), output.begin() + d_access_len);
        break;
      // if fix mode, replace syncwords in output with syncwords from memory
      case SYNC_FIX:
        for (int i = d_access_len - 1; i >= 0; i--) {
            uint8_t val = (d_reversed_vector[0]) ? ((d_access_code >> i) ^ 0x1)
                                                 : ((d_access_code >> i) & 0x1);

            output[d_access_len - i - 1] = val;
        }

        output.resize(output.size() - d_tail_len);
        for (int i = d_tail_len - 1; i >= 0; i--) {
            uint8_t val = (d_reversed_vector[0]) ? ((d_tail_sync >> i) ^ 0x1)
                                                 : ((d_tail_sync >> i) & 0x1);

            output.push_back(val);
        }
      case SYNC_KEEP:
        break;
    }
    // publish PDU
    this->message_port_pub(
        PMTCONSTSTR__pdu_out(),
        pmt::cons(meta_dict,
                  pmt::init_u8vector(output.size(), (const unsigned char*)&output[0])));

    d_burst_counter++;
}

int access_code_to_pdu_impl::work(int noutput_items,
                                  gr_vector_const_void_star& input_items,
                                  gr_vector_void_star& output_items)
{
    const uint8_t* in = (const uint8_t*)input_items[0];

    uint8_t new_bit;
//    uint64_t data_temp;

    // for each sample in the input buffer
    for (int i = 0; i < noutput_items; i++) {
        // read in new bit
        new_bit = in[i] & 0x1;

        // check for access code if:
        //  - block isn't in locked state, or is locked and awaiting new burst
        //  - data register has read in enough bits
        if (!d_lock || (d_lock && d_burst_vector.empty())) {
            if (d_nread >= d_access_len) {
                // if an access code was found
                if (check_access_code()) {
                    switch (d_readmode) {
                    // if in reset mode, dump any pending burst and flush output buffer
                    case READ_RESET:
                        d_reversed_vector.clear();
                        d_burst_vector.clear();
                        d_output.clear();
                        break;
                    // if in strict mode, turn on lock
                    case READ_STRICT:
                        d_lock = true;
                        break;
                    // if in permissive mode, do nothing extra
                    case READ_PERMISSIVE:
                        break;
                    }
                    // store boolean keeping track of bit-reversed status
                    d_reversed_vector.push_back(d_reversed);
                    if (d_burst_vector.empty()) {
                        // if this is the only burst that hasn't been published yet
                        // push the access code in the data register to the output buffer
                        for (int j = d_access_len - 1; j >= 0; j--) {
                            d_output.push_back((d_data_reg >> j) & 0x1);
                        }
                    }
                    // increment number of bursts by 1
                    d_burst_vector.push_back(d_access_len);
                    // if no code was found, unlock
                } else {
                    d_lock = false;
                }
            }
        }

        // update data register
        d_data_reg = (d_data_reg << 1) | new_bit;
        // update number of bits read in
        d_nread++;

        // if there is at least one pending burst, handle output buffer
        if (d_burst_vector.size()) {
            d_output.push_back(new_bit);
            // increment how many bits have been appended for each burst
            for (size_t ii = 0; ii < d_burst_vector.size(); ii++) {
                d_burst_vector[ii]++;
            }
            // if the oldest burst has reached the burst length
            if (d_burst_vector[0] == d_burst_len) {
                // publish the output buffer if there is a valid tail sync
                // tail sync must be bit-reversed if access code is bit-reversed
                if (check_tail_sync()) {
                    publish_message();
                    // if locked, set d_nread to 0 so it will read in enough bits
                    // to check the next access word
                    if (d_lock) {
                        d_nread = 0;
                    }
                } else {
                    // if the tail sync failed, unlock
                    d_lock = false;
                }
                // either way, remove burst under test from burst vector
                d_burst_vector.erase(d_burst_vector.begin());
                d_reversed_vector.erase(d_reversed_vector.begin());
                // if there are no other bursts, flush output buffer
                if (d_burst_vector.empty()) {
                    d_output.clear();
                } else {
                    // otherwise, remove data bits from the data buffer until the next
                    // syncword
                    d_output.erase(d_output.begin(),
                                   d_output.begin() + (d_burst_len - d_burst_vector[0]));
                }
            }
        }
        d_bit_index++;
    }
    return noutput_items;
}

} /* namespace pdu_utils */
} /* namespace gr */
