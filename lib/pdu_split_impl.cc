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

#include "pdu_split_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace pdu_utils {

pdu_split::sptr pdu_split::make(bool pass_empty_data)
{
    return gnuradio::get_initial_sptr(new pdu_split_impl(pass_empty_data));
}

/*
 * The private constructor
 */
pdu_split_impl::pdu_split_impl(bool pass_empty_data)
    : gr::block("pdu_split", io_signature::make(0, 0, 0), io_signature::make(0, 0, 0)),
      d_pass_empty_data(pass_empty_data)
{
    message_port_register_in(PMTCONSTSTR__PDU_IN);
    set_msg_handler(PMTCONSTSTR__PDU_IN,
                    boost::bind(&pdu_split_impl::handle_pdu, this, _1));
    message_port_register_out(PMTCONSTSTR__DICT);
    message_port_register_out(PMTCONSTSTR__DATA);
}

/*
 * Our virtual destructor.
 */
pdu_split_impl::~pdu_split_impl() {}

void pdu_split_impl::handle_pdu(pmt::pmt_t pdu)
{

    // make sure PDU data is formed properly
    if (!(pmt::is_pair(pdu))) {
        GR_LOG_WARN(d_logger, "received unexpected PMT (non-pair)");
        return;
    }

    pmt::pmt_t meta = pmt::car(pdu);
    pmt::pmt_t v_data = pmt::cdr(pdu);

    if ((!pmt::equal(meta, pmt::PMT_NIL)) | d_pass_empty_data) {
        message_port_pub(PMTCONSTSTR__DICT, pmt::car(pdu));
    }

    if (pmt::is_uniform_vector(v_data)) {
        if (pmt::length(v_data) | d_pass_empty_data) {
            message_port_pub(PMTCONSTSTR__DATA, pmt::cdr(pdu));
        }
    }
}

} /* namespace pdu_utils */
} /* namespace gr */
