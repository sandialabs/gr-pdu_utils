/* -*- c++ -*- */
/*
 * Copyright 2018-2021 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */


#ifndef INCLUDED_PDU_UTILS_CONSTANTS_H
#define INCLUDED_PDU_UTILS_CONSTANTS_H

#include <gnuradio/pdu_utils/api.h>
#include <pmt/pmt.h>

namespace gr {
namespace pdu_utils {

enum bit_order { BIT_ORDER_MSB_FIRST = 0, BIT_ORDER_LSB_FIRST = 1 };
enum pack_unpack_mode { MODE_UNPACK_BYTE = 0, MODE_PACK_BYTE = 1, MODE_BITSWAP_BYTE = 2 };
enum input_type {
    INPUTTYPE_UNPACKED_BYTE = 0,
    INPUTTYPE_PACKED_BYTE = 1,
    INPUTTYPE_FLOAT = 1
};
enum early_burst_behavior {
    EARLY_BURST_BEHAVIOR__APPEND = 0,
    EARLY_BURST_BEHAVIOR__DROP = 1,
    EARLY_BURST_BEHAVIOR__BALK = 2
};

// static const PMT interned string getters
// TODO: many of these are only used in one block. they really belong there
// unless they are truly namespace scoped for some reason
PDU_UTILS_API const pmt::pmt_t PMTCONSTSTR__msg();
PDU_UTILS_API const pmt::pmt_t PMTCONSTSTR__in();
PDU_UTILS_API const pmt::pmt_t PMTCONSTSTR__out();
PDU_UTILS_API const pmt::pmt_t PMTCONSTSTR__pdu_in();
PDU_UTILS_API const pmt::pmt_t PMTCONSTSTR__pdu_out();
PDU_UTILS_API const pmt::pmt_t PMTCONSTSTR__cpdus();
PDU_UTILS_API const pmt::pmt_t PMTCONSTSTR__fpdus();
PDU_UTILS_API const pmt::pmt_t PMTCONSTSTR__burst_time();
PDU_UTILS_API const pmt::pmt_t PMTCONSTSTR__start_time();
PDU_UTILS_API const pmt::pmt_t PMTCONSTSTR__start_time_offset();
PDU_UTILS_API const pmt::pmt_t PMTCONSTSTR__rx_time();
PDU_UTILS_API const pmt::pmt_t PMTCONSTSTR__tx_time();
PDU_UTILS_API const pmt::pmt_t PMTCONSTSTR__uhd_time_tuple();
PDU_UTILS_API const pmt::pmt_t PMTCONSTSTR__dict();
PDU_UTILS_API const pmt::pmt_t PMTCONSTSTR__data();
PDU_UTILS_API const pmt::pmt_t PMTCONSTSTR__emit();
PDU_UTILS_API const pmt::pmt_t PMTCONSTSTR__head();
PDU_UTILS_API const pmt::pmt_t PMTCONSTSTR__tail();
PDU_UTILS_API const pmt::pmt_t PMTCONSTSTR__ctrl();
PDU_UTILS_API const pmt::pmt_t PMTCONSTSTR__conf();
PDU_UTILS_API const pmt::pmt_t PMTCONSTSTR__val();
PDU_UTILS_API const pmt::pmt_t PMTCONSTSTR__key();
PDU_UTILS_API const pmt::pmt_t PMTCONSTSTR__set_val();
PDU_UTILS_API const pmt::pmt_t PMTCONSTSTR__set_key();
PDU_UTILS_API const pmt::pmt_t PMTCONSTSTR__bursts();
PDU_UTILS_API const pmt::pmt_t PMTCONSTSTR__detects();
PDU_UTILS_API const pmt::pmt_t PMTCONSTSTR__debug();
PDU_UTILS_API const pmt::pmt_t PMTCONSTSTR__zeroX();
PDU_UTILS_API const pmt::pmt_t PMTCONSTSTR__window();
PDU_UTILS_API const pmt::pmt_t PMTCONSTSTR__burst_id();
PDU_UTILS_API const pmt::pmt_t PMTCONSTSTR__burst_index();
PDU_UTILS_API const pmt::pmt_t PMTCONSTSTR__eob_offset();
PDU_UTILS_API const pmt::pmt_t PMTCONSTSTR__eob_alignment();
PDU_UTILS_API const pmt::pmt_t PMTCONSTSTR__sample_rate();
PDU_UTILS_API const pmt::pmt_t PMTCONSTSTR__symbol_rate();
PDU_UTILS_API const pmt::pmt_t PMTCONSTSTR__tx_sob();
PDU_UTILS_API const pmt::pmt_t PMTCONSTSTR__tx_eob();
PDU_UTILS_API const pmt::pmt_t PMTCONSTSTR__pdu_num();
PDU_UTILS_API const pmt::pmt_t PMTCONSTSTR__time_type();
PDU_UTILS_API const pmt::pmt_t PMTCONSTSTR__wall_clock_time();
PDU_UTILS_API const pmt::pmt_t PMTCONSTSTR__duration();
PDU_UTILS_API const pmt::pmt_t PMTCONSTSTR__set_trigger_tag();
PDU_UTILS_API const pmt::pmt_t PMTCONSTSTR__set_arming_tag();
PDU_UTILS_API const pmt::pmt_t PMTCONSTSTR__set_tx_limit();
PDU_UTILS_API const pmt::pmt_t PMTCONSTSTR__set_delays();
PDU_UTILS_API const pmt::pmt_t PMTCONSTSTR__set_message();
PDU_UTILS_API const pmt::pmt_t PMTCONSTSTR__set_holdoff();
PDU_UTILS_API const pmt::pmt_t PMTCONSTSTR__set_armed();
PDU_UTILS_API const pmt::pmt_t PMTCONSTSTR__trigger_now();
PDU_UTILS_API const pmt::pmt_t PMTCONSTSTR__system();
PDU_UTILS_API const pmt::pmt_t PMTCONSTSTR__phase_inc();
PDU_UTILS_API const pmt::pmt_t PMTCONSTSTR__bit_reversed();
PDU_UTILS_API const pmt::pmt_t PMTCONSTSTR__bit_index();


enum message_trigger_mode : uint64_t { TX_UNLIMITED = 0xFFFFFFFFFFFFFFFF, TX_OFF = 0 };

// noise generation distributions
enum noise_dist { UNIFORM = 0, GAUSSIAN = 1 };

enum window_type { TUKEY_WIN = 0, GAUSSIAN_WIN = 1 }; // end enum window_type

//! pdu align modes
enum align_modes {
    //! do not emit a packet if sync word is not found
    ALIGN_DROP = 0,

    //! forward packet if sync word is not found
    ALIGN_FORWARD = 1,

    //! emit empty pdu if sync word is not found
    ALIGN_EMPTY = 2
};


/*
 * Align match modes
 */
enum align_match_mode {
    ALIGN_FIRST_MATCH , 
    ALIGN_BEST_MATCH 
};


//! access code to pdu read mode
enum read_mode {
    // lock when sync word is found, unlock when de-synced
    READ_STRICT,

    // declare a new burst, output a PDU for each instance
    // of the sync word found
    READ_PERMISSIVE,

    // drop all samples from the previously declared burst,
    // start over
    READ_RESET
};


//! access code to pdu syncword publishing mode
enum sync_mode {
    // publish burst with syncwords as is
    SYNC_KEEP,

    // publish burst with corrected bits
    SYNC_FIX,

    // publish burst after truncating syncwords
    SYNC_DISCARD
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_CONSTANTS_H */
