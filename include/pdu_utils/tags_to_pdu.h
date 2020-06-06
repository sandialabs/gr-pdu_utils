/* -*- c++ -*- */
/*
 * Copyright 2018 National Technology & Engineering Solutions of Sandia, LLC (NTESS).
 * Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government retains
 * certain rights in this software.
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

#ifndef INCLUDED_PDU_UTILS_TAGS_TO_PDU_H
#define INCLUDED_PDU_UTILS_TAGS_TO_PDU_H

#include <gnuradio/sync_block.h>
#include <pdu_utils/api.h>

namespace gr {
namespace pdu_utils {

/*!
 * \brief Tags to PDU
 * \ingroup pdu_utils
 *
 * This block will generate a PDU (of up to Max PDU Size) based on input
 * tags. No tag alignment in input buffer is necessary.
 *
 *
 */
template <class T>
class PDU_UTILS_API tags_to_pdu : virtual public gr::sync_block
{
public:
    typedef boost::shared_ptr<tags_to_pdu<T>> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of pdu_utils::tags_to_pdu.
     *
     * @param start_tag -
     * @param end_tag -
     * @param max_pdu_size -
     * @param samp_rate -
     * @param prepend -
     * @param pub_sobs -
     * @param tail_size -
     * @param start_time -
     */
    static sptr make(pmt::pmt_t start_tag,
                     pmt::pmt_t end_tag,
                     uint32_t max_pdu_size,
                     double samp_rate,
                     std::vector<T> prepend,
                     bool pub_sobs,
                     uint32_t tail_size,
                     double start_time);

    virtual void set_eob_parameters(uint32_t, uint32_t) = 0;
    virtual uint32_t get_eob_offset(void) = 0;
    virtual uint32_t get_eob_alignment(void) = 0;

    virtual void set_start_tag(pmt::pmt_t tag) = 0;
    virtual void set_end_tag(pmt::pmt_t tag) = 0;
    virtual void set_time_tag_key(pmt::pmt_t tag) = 0;
    virtual void set_prepend(std::vector<T> prepend) = 0;
    virtual void set_tail_size(uint32_t size) = 0;
    virtual void set_max_pdu_size(uint32_t size) = 0;
    virtual void set_samp_rate(double) = 0;
    virtual void set_start_time(double) = 0;
    virtual void publish_sob_msgs(bool) = 0;
    virtual void enable_time_debug(bool) = 0;
};

typedef tags_to_pdu<unsigned char> tags_to_pdu_b;
typedef tags_to_pdu<short> tags_to_pdu_s;
typedef tags_to_pdu<float> tags_to_pdu_f;
typedef tags_to_pdu<gr_complex> tags_to_pdu_c;
} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_TAGS_TO_PDU_B_H */
