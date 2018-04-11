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

/* @WARNING@ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "@IMPL_NAME@.h"


namespace gr {
  namespace pdu_utils {


    @BASE_NAME@::sptr
    @BASE_NAME@::make(uint32_t take, uint32_t skip)
    {
      return gnuradio::get_initial_sptr
        (new @IMPL_NAME@(take, skip));
    }

    /*
     * The private constructor
     */
    @IMPL_NAME@::@IMPL_NAME@(uint32_t take, uint32_t skip)
      : gr::sync_block("@NAME@",
            gr::io_signature::make(1, 1, sizeof(@I_TYPE@)),
            gr::io_signature::make(0, 0, 0)),
        d_take(take),
        d_skip(skip),
        d_next(take),
        d_triggered(true),
        d_burst_counter(0),
        d_prev_byte(0)
    {
      if ( d_take == 0 ) {
        GR_LOG_FATAL(d_logger, "TAKE value too small, must be > 0");
        throw std::invalid_argument("TAKE value out of bounds");
      }
      if ( d_take > TAKESKIP_MAXIMUM_PDU_SIZE ) {
        GR_LOG_FATAL(d_logger, boost::format("TAKE value too large, must be less than %") % TAKESKIP_MAXIMUM_PDU_SIZE);
        throw std::invalid_argument("TAKE value out of bounds");
      }

      d_vector.clear();
      d_meta_dict = pmt::make_dict();
      message_port_register_out(PMTCONSTSTR__PDU_OUT);

      GR_LOG_INFO(d_logger, "Starting Take Skip PDU Generator!");
    }


    /*
     * Our virtual destructor.
     */
     @IMPL_NAME@::~@IMPL_NAME@()
    {
    }


    void
    @IMPL_NAME@::publish_message()
    {
      d_meta_dict = pmt::dict_add(d_meta_dict, pmt::intern("pdu_num"), pmt::from_uint64(d_burst_counter));
      //std::cout << "CPP: sending burst number " << d_burst_counter << " of length " << d_vector.size() << std::endl;
      message_port_pub(PMTCONSTSTR__PDU_OUT, (pmt::cons(d_meta_dict, pmt::init_@PMT_TYPE@vector(d_vector.size(), (const @I_TYPE@ *)&d_vector[0]) )));

      // prepare for next burst
      d_burst_counter++;
      d_triggered = false;
      d_vector.clear();
    }


    int
    @IMPL_NAME@::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      gr::thread::scoped_lock l(d_setlock);

      const @I_TYPE@ *in = (const @I_TYPE@ *) input_items[0];
      uint64_t consumed = noutput_items;
      bool start_stop = false;

      // figure out where the next start/stop point is
      if ( d_next <= (nitems_read(0) + noutput_items) ) {
        // we are going to start/stop during this buffer
        consumed = d_next - nitems_read(0);
        start_stop = true;
        if ( d_triggered ) {
          d_next += d_skip;
        } else {
          d_next += d_take;
        }
      }

      if ( d_triggered ) {
        // we are saving data
        //std::cout << "    adding  " << consumed << " items to the queue" << std::endl;
        d_vector.insert(d_vector.end(), &in[0], &in[consumed]);
        if ( start_stop ) {
          //std::cout << "    publishing message with " << d_vector.size() << " items" << std::endl;
          publish_message();
          if ( d_skip == 0 ) {
            d_triggered = true;
            d_next += d_take;
          }
        }
      } else {
        // if we are not saving data but need to start next time:
        if ( start_stop ) d_triggered = true;
      }

      // Tell runtime system how many output items we produced.
      return consumed;
    }


    void
    @IMPL_NAME@::set_take(uint32_t take)
    {
      gr::thread::scoped_lock l(d_setlock);

      d_take = take;
    }


    void
    @IMPL_NAME@::set_skip(uint32_t skip)
    {
      gr::thread::scoped_lock l(d_setlock);

      d_skip = skip;
    }

  } /* namespace pdu_utils */
} /* namespace gr */
