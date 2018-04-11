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

#ifndef @GUARD_NAME@
#define @GUARD_NAME@

#include <pdu_utils/@BASE_NAME@.h>
#include <pdu_utils/constants.h>

namespace gr {
  namespace pdu_utils {

    class @IMPL_NAME@ : public @BASE_NAME@
    {
     private:
      pmt::pmt_t d_sob_tag_key;
      pmt::pmt_t d_eob_tag_key;
      pmt::pmt_t d_time_tag_key;
      uint32_t d_max_pdu_size;
      double d_samp_rate;
      bool d_pub_sobs;
      std::vector<@I_TYPE@> d_prepend;
      uint32_t d_tail_size;
      uint64_t d_known_time_int_sec;  // known integer seconds of a particular item
      double d_known_time_frac_sec;   // known fractional seconds of a particular item
      uint64_t d_known_time_offset;   // known item offset of a particular item

      uint32_t d_eob_alignment;
      uint32_t d_eob_offset;
      uint64_t d_burst_counter;
      uint64_t d_sob_tag_offset;

      std::vector<@I_TYPE@> d_vector;
      pmt::pmt_t d_meta_dict;
      std::vector<tag_t> d_tags;
      tag_t d_tag;
      bool d_triggered;

      bool d_wall_clock_time;
      boost::posix_time::ptime d_epoch;

      enum TAG_TYPE { NONE=0, SOB, EOB } d_tag_type;

      void publish_message(void);
      void set_known_time_offset(uint64_t, double, uint64_t);


     public:
      @IMPL_NAME@(pmt::pmt_t start_tag, pmt::pmt_t end_tag, uint32_t max_pdu_size, double samp_rate, std::vector<@I_TYPE@> prepend, bool pub_sobs, uint32_t tail_size, double start_time);
      ~@IMPL_NAME@();

      // public callbacks
      void set_start_tag(pmt::pmt_t);
      void set_end_tag(pmt::pmt_t);
      void set_time_tag_key(pmt::pmt_t);
      void set_samp_rate(double);
      void set_start_time(double);
      void set_prepend(std::vector<@I_TYPE@>);
      void set_tail_size(uint32_t);
      void set_max_pdu_size(uint32_t);
      void publish_sob_msgs(bool pub) { d_pub_sobs = pub; };
      void set_eob_parameters(uint32_t, uint32_t);
      void enable_time_debug(bool);

      void handle_ctrl_msg(pmt::pmt_t ctrl_msg);

      uint32_t get_eob_offset() { return d_eob_offset; };
      uint32_t get_eob_alignment() { return d_eob_alignment; };

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace pdu_utils
} // namespace gr

#endif /* @GUARD_NAME@ */
