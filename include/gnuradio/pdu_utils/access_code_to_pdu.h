/* -*- c++ -*- */
/*
 * Copyright 2018, 2019, 2020, 2022 National Technology & Engineering Solutions of Sandia,
 * LLC (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_PDU_UTILS_ACCESS_CODE_TO_PDU_H
#define INCLUDED_PDU_UTILS_ACCESS_CODE_TO_PDU_H

#include <gnuradio/sync_block.h>
#include <gnuradio/pdu_utils/api.h>
#include <gnuradio/pdu_utils/constants.h>

namespace gr {
namespace pdu_utils {

/*!
 * \brief Access Code to PDU
 * \ingroup pdu_utils
 *
 * Parse an incoming stream of binary data for an access code, or head
 * syncword. Given a burst length in bits, the block will receive
 * that many samples, then check for a tail sync (if provided). If the check
 * succeeds, it publishes a PDU with the burst in the data. The block also
 * checks if the syncwords were bit-reversed and will returns a bit-corrected
 * burst, with a metadata tag notifying that the input was reversed.
 *
 * Additional parameters:
 * - threshold: the maximum Hamming distance in determining a detected sync word
 * - sync_mode: determines what the block does with the syncwords before publishing
 *     - keep: publishes the syncwords "as is"
 *     - discard: trims the syncwords prior to publishing
 *     - fix: if there were bit errors in the syncwords, correct them and publish
 * - read_mode: determines the behavior of the block once an access code is detected
 *     - strict: enter a locked state in which the block reads in the full burst
 *         without searching for syncwords; if a PDU is published, check for a
 *         syncword for a new burst immediately afterward; if a syncword check fails
 *         at any point, exit the locked state
 *     - permissive: every time the access code is found, treat each one as the start
 *         of a potential burst, regardless of the number of bits between them
 *     - reset: if an access code is found in the middle of a previous burst, discard
 *         the initial burst and start a new burst
 *
 */
class PDU_UTILS_API access_code_to_pdu : virtual public gr::sync_block
{
public:
    typedef std::shared_ptr<access_code_to_pdu> sptr;

    static sptr make(std::string access_code,
                     std::string tail_sync,
                     uint32_t burst_len,
                     uint32_t threshold,
                     sync_mode syncmode,
                     read_mode readmode);
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_ACCESS_CODE_TO_PDU_H */
