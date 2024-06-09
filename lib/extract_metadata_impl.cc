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

#include "extract_metadata_impl.h"
#include <gnuradio/io_signature.h>
#include <boost/format.hpp>
namespace gr {
namespace pdu_utils {

extract_metadata::sptr extract_metadata::make(pmt::pmt_t key, float scale, float offset)
{
    return gnuradio::make_block_sptr<extract_metadata_impl>(key, scale, offset);
}

/*
 * The private constructor
 */
extract_metadata_impl::extract_metadata_impl(pmt::pmt_t key, float scale, float offset)
    : gr::block(
          "extract_metadata", io_signature::make(0, 0, 0), io_signature::make(0, 0, 0)),
      d_key(key),
      d_scale(scale),
      d_offset(offset)
{
    message_port_register_in(PMTCONSTSTR__dict());
    set_msg_handler(PMTCONSTSTR__dict(),
                    [this](pmt::pmt_t msg) { this->handle_msg(msg); });
    message_port_register_out(PMTCONSTSTR__msg());
}

/*
 * Our virtual destructor.
 */
extract_metadata_impl::~extract_metadata_impl() {}


/*
 * Message handler. This is expected to handle PDUs (dict,vec pairs), and
 * individual dictionary messages.
 */
void extract_metadata_impl::handle_msg(pmt::pmt_t msg)
{
    pmt::pmt_t dict;

    if (pmt::is_dict(msg)) {
        dict = msg;
    } else if (pmt::is_pdu(msg)) {
        dict = pmt::car(msg);
    } else {
        // if not a dict or a pdu, drop the message
        return;
    }

    // we have a dictionary or a pair of which one element is a dict
    pmt::pmt_t value = pmt::dict_ref(msg, d_key, pmt::PMT_NIL);

    // if the lookup failed, treat as PDU and run the message handler again
    if (pmt::eqv(value, pmt::PMT_NIL)) {
        handle_msg(pmt::car(msg));

    } else {
        // set lock here as handle_msg() is called recursively
        gr::thread::scoped_lock l(d_setlock);

        // this part is just to attempt to apply scaling and offset
        if (pmt::is_number(value)) {
            if (pmt::is_real(value)) {
                try {
                    value = pmt::from_double(pmt::to_double(value) * d_scale + d_offset);
                } catch (const pmt::wrong_type& e) {
                    d_logger->error("was real...but double() failed");
                }
            } else if (pmt::is_uint64(value)) {
                try {
                    value = pmt::from_uint64(
                        uint64_t(pmt::to_uint64(value) * d_scale + d_offset));
                } catch (const pmt::wrong_type& e) {
                    d_logger->error("something went wrong getting uint64 {}",uint64_t(pmt::to_uint64(value) * d_scale + d_offset));
                }
            } else if (pmt::is_integer(value)) {
                try {
                    value =
                        pmt::from_long(long(pmt::to_long(value) * d_scale + d_offset));
                } catch (const pmt::wrong_type& e) {
                    d_logger->error("something went wrong getting long {}",long(pmt::to_long(value) * d_scale + d_offset));
                }
            } else if (pmt::is_complex(value)) {
                ;
            } else {
                d_logger->error("got a number...but not an int, real, complex");
            }
        }

        message_port_pub(PMTCONSTSTR__msg(), pmt::cons(d_key, value));
    }
}


void extract_metadata_impl::set_key(pmt::pmt_t key)
{
    gr::thread::scoped_lock l(d_setlock);

    d_key = key;
}


void extract_metadata_impl::set_scale(float scale)
{
    gr::thread::scoped_lock l(d_setlock);

    d_scale = scale;
}


void extract_metadata_impl::set_offset(float offset)
{
    gr::thread::scoped_lock l(d_setlock);

    d_offset = offset;
}

} /* namespace pdu_utils */
} /* namespace gr */
