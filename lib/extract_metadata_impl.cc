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
#include "extract_metadata_impl.h"

namespace gr {
  namespace pdu_utils {

    extract_metadata::sptr
    extract_metadata::make(pmt::pmt_t key, float scale, float offset)
    {
      return gnuradio::get_initial_sptr
        (new extract_metadata_impl(key, scale, offset));
    }

    /*
     * The private constructor
     */
    extract_metadata_impl::extract_metadata_impl(pmt::pmt_t key, float scale, float offset)
      : gr::block("extract_metadata",
              io_signature::make (0, 0, 0),
              io_signature::make (0, 0, 0)),
        d_key(key),
        d_scale(scale),
        d_offset(offset)
    {
      message_port_register_in(PMTCONSTSTR__DICT);
      set_msg_handler(PMTCONSTSTR__DICT, boost::bind(&extract_metadata_impl::handle_msg, this, _1));
      message_port_register_out(PMTCONSTSTR__MSG);
    }

    /*
     * Our virtual destructor.
     */
    extract_metadata_impl::~extract_metadata_impl()
    {
    }


    /*
     * Message handler. This is expected to handle PDUs (dict,vec pairs), and
     * individual dictionary messages.
     */
    void
    extract_metadata_impl::handle_msg(pmt::pmt_t msg)
    {
      // is_pair() will pass both dictionaries and pairs (possible PDUs...)
      if (!pmt::is_pair(msg)) {
        return;
      }

      try {
        // Will fail if msg is not a dictionary or PDU (dict/pmt pair)
        pmt::pmt_t x = pmt::dict_keys(msg);
      } catch (const pmt::wrong_type &e) {
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
            } catch (const pmt::wrong_type &e) {
              GR_LOG_ERROR(d_logger, "was real...but double() failed");
            }
          } else if (pmt::is_uint64(value)) {
            try {
              value = pmt::from_uint64(uint64_t (pmt::to_uint64(value) * d_scale + d_offset));
            } catch (const pmt::wrong_type &e) {
              GR_LOG_ERROR(d_logger, boost::format("something went wrong getting uint64") % value);
            }
          } else if (pmt::is_integer(value)) {
            try {
              value = pmt::from_long(long (pmt::to_long(value) * d_scale + d_offset));
            } catch (const pmt::wrong_type &e) {
              GR_LOG_ERROR(d_logger, boost::format("something went wrong getting long") % value);
            }
          } else if (pmt::is_complex(value)) {
            ;
          } else {
            GR_LOG_ERROR(d_logger, boost::format("got a number...but not an int, real, complex"));
          }
        }

        message_port_pub(PMTCONSTSTR__MSG, pmt::cons(d_key, value));
      }
    }


    void
    extract_metadata_impl::set_key(pmt::pmt_t key)
    {
      gr::thread::scoped_lock l(d_setlock);

      d_key = key;
    }


    void
    extract_metadata_impl::set_scale(float scale)
    {
      gr::thread::scoped_lock l(d_setlock);

      d_scale = scale;
    }


    void
    extract_metadata_impl::set_offset(float offset)
    {
      gr::thread::scoped_lock l(d_setlock);

      d_offset = offset;
    }

  } /* namespace pdu_utils */
} /* namespace gr */
