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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "pdu_range_filter_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace pdu_utils {

pdu_range_filter::sptr
pdu_range_filter::make(pmt::pmt_t key, double min, double max, bool invert)
{
    return gnuradio::get_initial_sptr(new pdu_range_filter_impl(key, min, max, invert));
}

/*
 * The private constructor
 */
pdu_range_filter_impl::pdu_range_filter_impl(pmt::pmt_t key,
                                             double min,
                                             double max,
                                             bool invert)
    : gr::block("pdu_range_filter",
                gr::io_signature::make(0, 0, 0),
                gr::io_signature::make(0, 0, 0)),
      d_key(key),
      d_min(min),
      d_max(max),
      d_invert(invert)
{
    message_port_register_in(PMTCONSTSTR__PDU_IN);
    message_port_register_out(PMTCONSTSTR__PDU_OUT);
    set_msg_handler(PMTCONSTSTR__PDU_IN,
                    boost::bind(&pdu_range_filter_impl::pdu_handler, this, _1));
}

/*
 * Our virtual destructor.
 */
pdu_range_filter_impl::~pdu_range_filter_impl() {}

void pdu_range_filter_impl::pdu_handler(pmt::pmt_t pdu)
{

    pmt::pmt_t metadata = pmt::car(pdu);
    bool output = d_invert;

    // make sure metadata is a dictionary and has the right key
    if (pmt::is_dict(metadata) && dict_has_key(metadata, d_key)) {
        pmt::pmt_t pmt_val = pmt::dict_ref(metadata, d_key, pmt::PMT_NIL);
        // get the value
        double val;
        if (pmt::is_integer(pmt_val)) {
            val = (double)pmt::to_long(pmt_val);
        } else if (pmt::is_uint64(pmt_val)) {
            val = (double)pmt::to_uint64(pmt_val);
        } else if (pmt::is_real(pmt_val)) {
            val = pmt::to_double(pmt_val);
        } else {
            printf("WARNING: PDU metadata value is not a number, dropping PDU\n");
            return;
        }
        // if the value is inside the range, let the packet through
        if (val >= d_min && val <= d_max) {
            output = !d_invert;
        }
    }

    if (output) {
        message_port_pub(PMTCONSTSTR__PDU_OUT, pdu);
    }
}

} /* namespace pdu_utils */
} /* namespace gr */
