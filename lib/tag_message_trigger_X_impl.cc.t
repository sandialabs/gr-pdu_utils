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
    @BASE_NAME@::make(pmt::pmt_t trigger_key, pmt::pmt_t arming_key, pmt::pmt_t message, uint32_t holdoff, double samp_rate, double start_time, double delay_time, bool tpdu_mode)
    {
      return gnuradio::get_initial_sptr
        (new @IMPL_NAME@(trigger_key, arming_key, message, holdoff, samp_rate, start_time, delay_time, tpdu_mode));
    }

    /*
     * The private constructor
     */
    @IMPL_NAME@::@IMPL_NAME@(pmt::pmt_t trigger_key, pmt::pmt_t arming_key, pmt::pmt_t message, uint32_t holdoff, double samp_rate, double start_time, double delay_time, bool tpdu_mode)
      : gr::sync_block("@NAME@",
              gr::io_signature::make(1, 1, sizeof(@I_TYPE@)),
              gr::io_signature::make(0, 0, 0)),
        d_trigger_key(trigger_key),
        d_arming_key(arming_key),
        d_time_tag_key(PMTCONSTSTR__RX_TIME),
        d_fire_at_will(false),
        d_armed(false),
        d_message(message),
        d_last_trigger_offset(0),
        d_samp_rate(samp_rate),
        d_start_time(start_time),
        d_tpdu_mode(tpdu_mode),
        d_tx_limit(TX_UNLIMITED)
    {
      set_start_time(start_time);
      set_delay_time(delay_time);
      set_holdoff(holdoff);

      if (d_tpdu_mode) {
        GR_LOG_NOTICE(d_logger, boost::format("started at time %f in timed PDU mode") % start_time);
      } else {
        GR_LOG_NOTICE(d_logger, boost::format("started at time %f in normal message mode") % start_time);
      }

      if (pmt::eq(d_arming_key, pmt::PMT_NIL)) {
        GR_LOG_WARN(d_logger, "operating in always armed mode");
        d_fire_at_will = true;
      } else {
        GR_LOG_WARN(d_logger, boost::format("operating with arming key \"%s\"") % pmt::symbol_to_string(arming_key));
      }

      message_port_register_in(PMTCONSTSTR__CTRL);
      set_msg_handler(PMTCONSTSTR__CTRL,
          boost::bind(&@IMPL_NAME@::control_input, this, _1));
      message_port_register_out(PMTCONSTSTR__MSG);
    }

    /*
     * Our virtual destructor.
     */
    @IMPL_NAME@::~@IMPL_NAME@()
    {
    }


    // TODO: Protect every PMT defrefernce in this method from errors
    void
    @IMPL_NAME@::control_input(pmt::pmt_t msg) {
      // command inputs should be in the form of a pair - the command name
      // followd by any necessary arguments as a pair (or tuple if more than
      // two) - if no arguments are necessary the args should be pmt::PMT_NIL
      pmt::pmt_t command = pmt::car(msg);
      pmt::pmt_t cmd_args = pmt::cdr(msg);

      if (pmt::eqv(command, PMTCONSTSTR__SET_TRIGGER_KEY)) {
        // command args: {trigger key}
        set_trigger_key(cmd_args);
      } else if (pmt::eqv(command, PMTCONSTSTR__SET_ARMING_KEY)) {
        // command args: {arming key}
        set_arming_key(cmd_args);
      } else if (pmt::eqv(command, PMTCONSTSTR__SET_MESSAGE)) {
        // command args: {pdu message}
        set_message(cmd_args);
      } else if (pmt::eqv(command, PMTCONSTSTR__SET_DELAYS)) {
        // command args: {delay_secs}
        set_delay_time(pmt::to_double(cmd_args));
      } else if (pmt::eqv(command, PMTCONSTSTR__SET_TX_LIMIT)) {
        // command args: {limit}
        set_tx_limit(pmt::to_uint64(cmd_args));
      } else if (pmt::eqv(command, PMTCONSTSTR__SET_HOLDOFF)) {
        // command args: {holdoff}
        set_holdoff(pmt::to_uint64(cmd_args));
      } else if (pmt::eqv(command, PMTCONSTSTR__SET_ARMED)) {
        // command args: {arming_offset}
        d_armed = true;
        if (pmt::eqv(cmd_args, pmt::PMT_NIL)) {
          // if the arming sample was not provided, just use the num of items read
          d_disarm_offset = nitems_read(0) + d_holdoff;
        } else {
          d_disarm_offset = pmt::to_uint64(cmd_args) + d_holdoff;
        }
      } else if (pmt::eqv(command, PMTCONSTSTR__TRIGGER_NOW)) {
        // if the argument is PMT_NIL send the configured message, otherwise send the args
        if (pmt::eqv(cmd_args, pmt::PMT_NIL)) {
          // in timed pdu mode, send an untimed pdu assuming the message is a uniform vector (TODO: WHY??)
          if (d_tpdu_mode) {
            message_port_pub(PMTCONSTSTR__MSG, pmt::cons(pmt::make_dict(), pmt::cdr(d_message)));
          // in normal mode, send the message
          } else {
            message_port_pub(PMTCONSTSTR__MSG, d_message);
          }
        } else {
          message_port_pub(PMTCONSTSTR__MSG, cmd_args);
        }
        d_last_trigger_offset = nitems_read(0);
      }
    }


    int
    @IMPL_NAME@::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      gr::thread::scoped_lock l(d_setlock);

      const @I_TYPE@ *in = (const @I_TYPE@ *) input_items[0];

      uint32_t consumed = noutput_items;
      uint64_t a_start = nitems_read(0);
      uint64_t a_end = a_start + noutput_items;

      // get all tags:
      get_tags_in_range(d_tags, 0, a_start, a_end);

      // find relavant tags:
      for (int ii = 0; ii < d_tags.size(); ii++) {
        d_tag = d_tags[ii];
        if (pmt::eqv(d_tag.key, d_arming_key)) {
          d_armed = true;
          // the disarm offset never directly causes d_armed to be false
          // but it is checked as part of the arming verification stages
          d_disarm_offset = d_tag.offset + d_holdoff;

        } else if (pmt::eqv(d_tag.key, d_trigger_key)) {
          uint64_t trigger_offset = d_tag.offset;

          // series of checks to determine whether or not we should fire:
          // we are only armed if d_armed and arming holdoff are both OK
          bool arming_holdoff_ok = (trigger_offset < d_disarm_offset) || d_no_holdoff;
          bool firing_holdoff_ok = (trigger_offset > (d_last_trigger_offset + d_holdoff)) || d_no_holdoff;
          //std::cout << "trigger_offset: " << trigger_offset << ", d_last_trigger_offset: " << d_last_trigger_offset<< ", d_holdoff: " << d_holdoff << std::endl;
          //std::cout << "DBG: armed: " << d_armed << ", arm HO OK: " << arming_holdoff_ok << ", FAW: " << d_fire_at_will << ", fire HO OK: " << firing_holdoff_ok << ", offset = " << trigger_offset << "/" << d_last_trigger_offset << std::endl;

          // are we armed or in fire-at-will mode and ready to actually fire?
          if (((d_armed && arming_holdoff_ok) || d_fire_at_will) && firing_holdoff_ok) {
            //std::cout << "sending msg at trigger offset " << trigger_offset << std::endl;

            // if we have reached the transmit limit, disarm and continue
            if (d_tx_limit == 0) {
              GR_LOG_NOTICE(d_logger, "not firing, TX Limit reached");
              d_armed = false;

            // otherwise fire the burst
            } else {

              // if we are not in unlimited mode, decrement the transmit limit
              if (d_tx_limit != TX_UNLIMITED) { d_tx_limit--; }

              // if we are triggered and are in normal mode, send just the msg
              if (!d_tpdu_mode) {
                message_port_pub(PMTCONSTSTR__MSG, d_message);
                d_last_trigger_offset = trigger_offset;

              // triggered in timed PDU mode
              } else  {

                double delta;
                if (trigger_offset >= d_known_time_offset) {
                  delta = (trigger_offset - d_known_time_offset) / d_samp_rate;
                } else {
                  delta = -1.0 * (d_known_time_offset - trigger_offset) / d_samp_rate;
                }
                int int_delta = (int)delta;
                delta -= int_delta;

                uint64_t int_seconds = d_known_time_int_sec + int_delta + d_delay_time_int_sec;
                double frac_seconds = d_known_time_frac_sec + delta + d_delay_time_frac_sec;

                int residual = 0;
                if (frac_seconds > 1.0) {
                  residual = (int)frac_seconds;
                } else if (frac_seconds < 0.0) {
                  residual = -1 * ((int)(-1*frac_seconds));
                }
                if (residual != 0) {
                  frac_seconds -= residual;
                  int_seconds += residual;
                }
                pmt::pmt_t burst_time_tuple = pmt::make_tuple(pmt::from_uint64(int_seconds), pmt::from_double(frac_seconds));
                pmt::pmt_t dict = pmt::car(d_message);
                pmt::pmt_t vector = pmt::cdr(d_message);
                dict = pmt::dict_add(dict, pmt::mp("tx_time"), burst_time_tuple);
                message_port_pub(PMTCONSTSTR__MSG, pmt::cons(dict, vector));
                d_last_trigger_offset = trigger_offset;
              }

              d_armed = false;
            }

          // we aren't armed, do nothing
          } else {
            GR_LOG_INFO(d_logger, boost::format("trigger received at offset %d but not ready to fire") % d_tag.offset);
          }

        // if we get a rx_time tag from the USRP, use that to set timing offsets
      } else if (pmt::eqv(d_tags[ii].key, d_time_tag_key)) {
          set_known_time_offset(pmt::to_uint64(pmt::tuple_ref(d_tag.value, 0)),
                                pmt::to_double(pmt::tuple_ref(d_tag.value, 1)),
                                d_tag.offset);
          GR_LOG_INFO(d_logger, boost::format("got a time update ({%d  %f} at %d)") % d_known_time_int_sec % d_known_time_frac_sec % d_known_time_offset);
        }
      }

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }


    void
    @IMPL_NAME@::set_known_time_offset(uint64_t int_sec, double frac_sec, uint64_t offset)
    {
      // private method, called internally - no mutex
      d_known_time_int_sec = int_sec;
      d_known_time_frac_sec = frac_sec;
      d_known_time_offset = offset;
    }


//// thread safe setters ////
    void
    @IMPL_NAME@::set_samp_rate(double rate)
    {
      gr::thread::scoped_lock l(d_setlock);

      d_samp_rate = rate;
    }

    void
    @IMPL_NAME@::set_start_time(double start_time)
    {
      gr::thread::scoped_lock l(d_setlock);

      uint64_t sec = (uint64_t)start_time;
      double frac = (start_time - sec);
      set_known_time_offset(sec, frac, 0);
    }

    void
    @IMPL_NAME@::set_delay_time(double delay_time)
    {
      gr::thread::scoped_lock l(d_setlock);

      d_delay_time_int_sec = (uint64_t)delay_time;
      d_delay_time_frac_sec = (delay_time - d_delay_time_int_sec);
    }

    void
    @IMPL_NAME@::set_holdoff(uint32_t holdoff)
    {
      gr::thread::scoped_lock l(d_setlock);

      if (holdoff == 0) {
        d_holdoff = 0;
        d_no_holdoff = true;
      } else {
        d_holdoff = holdoff;
        d_no_holdoff = false;
      }
    }

    void
    @IMPL_NAME@::set_message(pmt::pmt_t msg)
    {
      gr::thread::scoped_lock l(d_setlock);

      d_message = msg;
    }

    void
    @IMPL_NAME@::set_tx_limit(uint64_t limit)
    {
      gr::thread::scoped_lock l(d_setlock);

      d_tx_limit = limit;
    }

  } /* namespace pdu_utils */
} /* namespace gr */
