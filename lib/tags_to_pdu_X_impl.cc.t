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
    @BASE_NAME@::make(pmt::pmt_t start_tag, pmt::pmt_t end_tag, uint32_t max_pdu_size, double samp_rate, std::vector<@I_TYPE@> prepend, bool pub_sobs, uint32_t tail_size, double start_time)
    {
      return gnuradio::get_initial_sptr
        (new @IMPL_NAME@(start_tag, end_tag, max_pdu_size, samp_rate, prepend, pub_sobs, tail_size, start_time));
    }

    /*
     * The private constructor
     */
    @IMPL_NAME@::@IMPL_NAME@(pmt::pmt_t start_tag, pmt::pmt_t end_tag, uint32_t max_pdu_size, double samp_rate, std::vector<@I_TYPE@> prepend, bool pub_sobs, uint32_t tail_size, double start_time)
      : gr::sync_block("@NAME@",
              gr::io_signature::make(1, 1, sizeof(@I_TYPE@)),
              gr::io_signature::make(0, 0, 0)),
        d_sob_tag_key(start_tag),
        d_eob_tag_key(end_tag),
        d_time_tag_key(PMTCONSTSTR__RX_TIME),
        d_max_pdu_size(max_pdu_size),
        d_samp_rate(samp_rate),
        d_prepend(prepend),
        d_pub_sobs(pub_sobs),
        d_tail_size(tail_size),
        d_triggered(false),
        d_burst_counter(0),
        d_sob_tag_offset(0),
        d_meta_dict(pmt::make_dict()),
        d_wall_clock_time(false)
    {
      // start times that will have roundoff issues are outside the intentions of this parameter
      set_start_time(start_time);
      set_eob_parameters(1, 0);
      GR_LOG_NOTICE(d_logger, boost::format("starting at time {%d %f}") % d_known_time_int_sec % d_known_time_frac_sec);

      GR_LOG_NOTICE(d_logger, boost::format("rate %0.12f") % d_samp_rate);

      // store the data to prepend
      //d_prepend.clear();
      //d_prepend.insert(d_prepend.end(), prepend.begin(), prepend.end());

      message_port_register_in(PMTCONSTSTR__CONF);
      set_msg_handler(PMTCONSTSTR__CONF,
          boost::bind(&@IMPL_NAME@::handle_ctrl_msg, this, _1));
      message_port_register_out(PMTCONSTSTR__PDU_OUT);
      message_port_register_out(PMTCONSTSTR__DETECTS);
    }

    /*
     * Our virtual destructor.
     */
    @IMPL_NAME@::~@IMPL_NAME@()
    {
    }


    void
    @IMPL_NAME@::handle_ctrl_msg(pmt::pmt_t ctrl_msg)
    {
      // if we do not receive a dictionary, don't do anything:
      if (!pmt::is_dict(ctrl_msg))
        return;

      // pairs pass is_dict, so check for those...this will fail for a pair
      // FIXME - This is WRONG! pairs will pass dict_keys()...just send in dictionaries!
      try {
        pmt::pmt_t keys = pmt::dict_keys(ctrl_msg);
        ctrl_msg = pmt::car(ctrl_msg);
      } catch (const pmt::wrong_type &e) {
        // So we fix it:
        GR_LOG_WARN(d_logger, "got a pair, not a dict, fixing");
        ctrl_msg = pmt::dict_add(pmt::make_dict(), pmt::car(ctrl_msg), pmt::cdr(ctrl_msg));
      }
      //std::cout << "got a dictionary with the following keys: " << pmt::dict_keys(ctrl_msg) << std::endl;

      //check dict for EOB offset command
      if (pmt::dict_has_key(ctrl_msg, PMTCONSTSTR__EOB_OFFSET)) {
        uint32_t new_eob_offset = pmt::to_uint64(pmt::dict_ref(
            ctrl_msg, PMTCONSTSTR__EOB_OFFSET, pmt::from_uint64(d_eob_offset)));
        set_eob_parameters(d_eob_alignment, new_eob_offset);
        GR_LOG_NOTICE(d_logger, boost::format("command received - set EOB tag offset to %d symbols") % d_eob_offset);
      }

      //check dict for EOB alignment command
      if (pmt::dict_has_key(ctrl_msg, PMTCONSTSTR__EOB_ALIGNMENT)) {
        uint32_t new_eob_alignment = pmt::to_uint64(pmt::dict_ref(
        ctrl_msg, PMTCONSTSTR__EOB_ALIGNMENT, pmt::from_uint64(d_eob_alignment)));
        if (new_eob_alignment > 0) {
          set_eob_parameters(new_eob_alignment, d_eob_offset);
          GR_LOG_NOTICE(d_logger, boost::format("command received - set EOB tag alignment to %d symbols") % d_eob_alignment);
        } else {
          GR_LOG_ERROR(d_logger, boost::format("command received - illegal value %d for EOB alignment, not setting") % new_eob_alignment);
        }
      }

      // handle other control messages...

    }


    void
    @IMPL_NAME@::publish_message()
    {
      /* determine the time. we always have the offset that the SOB tag
       * was received on, we know the rate, and also have a known time/offset
       * pair - calculate the delta and apply accordingly.
       */
      double delta;
      if (d_sob_tag_offset >= d_known_time_offset) {
        delta = (d_sob_tag_offset - d_known_time_offset) / d_samp_rate;
      } else {
        delta = -1.0 * (d_known_time_offset - d_sob_tag_offset) / d_samp_rate;
      }
      int int_delta = (int)delta;
      delta -= int_delta;

      uint64_t int_seconds = d_known_time_int_sec + int_delta;
      double frac_seconds = d_known_time_frac_sec + delta;

      // what if outside 1.0?
      if (frac_seconds >= 1.0) {
        frac_seconds -= 1.0;
        int_seconds += 1;
      }
      pmt::pmt_t time_tuple = pmt::make_tuple(pmt::from_uint64(int_seconds), pmt::from_double(frac_seconds));
      d_meta_dict = pmt::dict_add(d_meta_dict, PMTCONSTSTR__BURST_TIME, time_tuple);
      d_meta_dict = pmt::dict_add(d_meta_dict, PMTCONSTSTR__TIME_TYPE, PMTCONSTSTR__UHD_TIME_TUPLE); //TODO: remove, no longer necessary?
      d_meta_dict = pmt::dict_add(d_meta_dict, PMTCONSTSTR__PDU_NUM, pmt::from_uint64(d_burst_counter));
      if (d_wall_clock_time) {
        double t_now((boost::get_system_time() - d_epoch).total_microseconds()/1000000.0);
        d_meta_dict = pmt::dict_add(d_meta_dict, PMTCONSTSTR__WALL_CLOCK_TIME, pmt::from_double(t_now));
      }
      //std::cout << "CPP: sending burst number " << d_burst_counter << " of length " << d_vector.size() << " at time " << t_now << std::endl;
      if (d_vector.size() > d_tail_size)
        message_port_pub(PMTCONSTSTR__PDU_OUT, (pmt::cons(d_meta_dict, pmt::init_@PMT_TYPE@vector(d_vector.size()-d_tail_size, (const @I_TYPE@ *)&d_vector[0]) )));

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
      const @I_TYPE@ *in = (const @I_TYPE@ *) input_items[0];
      uint32_t consumed = noutput_items;

      uint64_t a_start = nitems_read(0);
      uint64_t a_end = a_start + noutput_items;

      //boost::posix_time::ptime time_now(boost::get_system_time());

      // get all tags:
      get_tags_in_range(d_tags, 0, a_start, a_end);

      // find first SOB/EOB tag and process an time/offset tags encountered
      d_tag_type = NONE;
      for (int ii = 0; ii < d_tags.size(); ii++) {
        d_tag = d_tags[ii];
        if (pmt::eqv(d_tag.key, d_sob_tag_key)) {
          d_tag_type = SOB;
          consumed = d_tag.offset - a_start + 1;
          break;
        } else if (pmt::eqv(d_tag.key, d_eob_tag_key)) {
          d_tag_type = EOB;
          consumed = d_tag.offset - a_start + 1;
          break;
        } else if (pmt::eqv(d_tag.key, d_time_tag_key)) {
          set_known_time_offset(pmt::to_uint64(pmt::tuple_ref(d_tag.value, 0)),
                                pmt::to_double(pmt::tuple_ref(d_tag.value, 1)),
                                d_tag.offset);
        }
      }

      /* if the system is already triggered (has received SOB), we will be
       * storing data until we reach an EOB tag or the maximum PDU size is
       * reached - regardless of what happens we will consume data up to and
       * including the received tag
       */
      if (d_triggered) {

        uint64_t last_addr_in_vec = (d_max_pdu_size - d_vector.size()) + a_start - 1;

        /* if we got an EOB/SOB tag, and the tag offset is before the max pdu
         * length, we need to take action on it
         */
        if ((d_tag_type != NONE) && (d_tag.offset <= last_addr_in_vec)) {

          if (d_tag_type == EOB) {

            // for EOB, always append data up to  (not including) tagged sample
            d_vector.insert(d_vector.end(), &in[0], &in[consumed-1]);

            // check to see if the EOB tag is correctly aligned within the burst
            if (((d_vector.size() - d_eob_offset) % d_eob_alignment) == 0) {

              publish_message();

            // if the tag is not byte-aligned, issue a warning and continue
            } else {
              // we will consume the tagged sample, so save it ot the data vector
              d_vector.push_back(in[consumed-1]);

              // The printout below is normal behavior...warning is not neecssary
              //std::cout << "EOB tag at " << d_tag.offset << " in burst " << d_burst_counter << " was not correctly aligned..." << std::endl;
            }

          // if we have received a second SOB tag, reset and dump previous data
          } else if (d_tag_type == SOB) {
            GR_LOG_ERROR(d_logger, boost::format("SOB tag received during burst %d at offset %d, previous burst dropped (%d tags total)") %
                                   d_burst_counter % d_tag.offset % d_tags.size());

            // prepare for next burst
            d_burst_counter++;
            d_triggered = false;
            d_vector.clear();

            // do not consume SOB item so it can be handled next work() call
            consumed--;
          }

        // otherwise, consume data and evaluate if we need to publish a message
        } else {
          // don't consume a SOB tag...
          consumed--;
          d_vector.insert(d_vector.end(), &in[0], &in[consumed]);
          if (d_vector.size() > d_max_pdu_size) {
            //std::cout << "PDU larger than " << d_max_pdu_size << ", writing it out with no EOB tag!" << std::endl;
            d_vector.resize(d_max_pdu_size);
            publish_message();
          }
        }

      /* if we are not triggered, we are waiting for an SOB tag, until that is
       * reached, save no data or do anything other than consume samples up to
       * and including the observed tag other than warn if EOB tags are seen
       */
      } else {
        if (d_tag_type == SOB) {
          // the sob tag offset will be used to determine received time
          d_sob_tag_offset = d_tag.offset;

          // store the prepended data
          d_vector.insert(d_vector.end(), d_prepend.begin(), d_prepend.end());
          d_vector.push_back(in[consumed - 1]);   // store the first bit, we will consume the tagged sample
          d_triggered = true;

          if (d_pub_sobs) {
            message_port_pub(PMTCONSTSTR__DETECTS, pmt::from_uint64(d_sob_tag_offset));
          }

        } else if (d_tag_type == EOB) {
          // receiving an EOB sequence while not triggered is just random chance. No warning necessary...
          GR_LOG_INFO(d_logger, boost::format("received unexpected EOB at offset %d") % d_tag.offset);

        } else {
          // do nothing
        }
      }

      return consumed;
    }


    void
    @IMPL_NAME@::set_start_tag(pmt::pmt_t tag)
    {
      gr::thread::scoped_lock l(d_setlock);

      d_sob_tag_key = tag;
    }


    void
    @IMPL_NAME@::set_end_tag(pmt::pmt_t tag)
    {
      gr::thread::scoped_lock l(d_setlock);

      d_eob_tag_key = tag;
    }


    void
    @IMPL_NAME@::set_time_tag_key(pmt::pmt_t tag)
    {
      gr::thread::scoped_lock l(d_setlock);

      d_time_tag_key = tag;
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
    @IMPL_NAME@::set_known_time_offset(uint64_t int_sec, double frac_sec, uint64_t offset)
    {
      d_known_time_int_sec = int_sec;
      d_known_time_frac_sec = frac_sec;
      d_known_time_offset = offset;
    }


    void
    @IMPL_NAME@::set_eob_parameters(uint32_t alignment, uint32_t offset)
    {
      gr::thread::scoped_lock l(d_setlock);

      d_eob_alignment = alignment;
      d_eob_offset = offset;
    }


    void
    @IMPL_NAME@::set_prepend(std::vector<@I_TYPE@> prepend)
    {
      gr::thread::scoped_lock l(d_setlock);

      d_prepend = prepend;
    }


    void
    @IMPL_NAME@::set_samp_rate(double rate)
    {
      gr::thread::scoped_lock l(d_setlock);

      d_samp_rate = rate;
    }


    void
    @IMPL_NAME@::set_tail_size(uint32_t size)
    {
      gr::thread::scoped_lock l(d_setlock);

      d_tail_size = size;
    }


    void
    @IMPL_NAME@::set_max_pdu_size(uint32_t size)
    {
      gr::thread::scoped_lock l(d_setlock);

      d_max_pdu_size = size;
    }


    void
    @IMPL_NAME@::enable_time_debug(bool enable) {
      if (enable) {
        boost::posix_time::ptime epoch(boost::gregorian::date(1970,1,1));
        d_epoch = epoch;
        d_wall_clock_time = true;
      } else {
        d_wall_clock_time = false;
      }
    }


  } /* namespace pdu_utils */
} /* namespace gr */
