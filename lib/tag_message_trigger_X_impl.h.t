/* -*- c++ -*- */
/*
 * <COPYRIGHT PLACEHOLDER>
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
      pmt::pmt_t d_message;      // message to send
      pmt::pmt_t d_trigger_key;  // tag key to trigger message
      pmt::pmt_t d_arming_key;   // tag key to arm message trigger
      pmt::pmt_t d_time_tag_key;   // tag key to arm message trigger
      double d_samp_rate;
      double d_start_time;
      bool d_tpdu_mode;
      uint64_t d_disarm_offset;  // offset the block should disarm at
      bool d_fire_at_will;       // if set, fire anytime trigger is received
      bool d_armed;              // armed state
      uint64_t d_tx_limit;       // prevent additional transmissions
      bool d_no_holdoff;
      uint32_t d_holdoff;        // holdoff - maximum arming time if there is a
                                 // a trigger, or if not minimum interval in
                                 // samples between trigger events
      uint64_t d_last_trigger_offset;

      uint64_t d_known_time_int_sec;  // known integer seconds of a particular item
      double d_known_time_frac_sec;   // known fractional seconds of a particular item
      uint64_t d_known_time_offset;   // known item offset of a particular item
      uint64_t d_delay_time_int_sec;  // timed PDU mode delay
      double d_delay_time_frac_sec;   // timed PDU mode delay

      tag_t d_tag;
      std::vector<tag_t> d_tags;

      void set_known_time_offset(uint64_t, double, uint64_t);


     public:
      @IMPL_NAME@(pmt::pmt_t trigger_key, pmt::pmt_t arming_key, pmt::pmt_t message, uint32_t holdoff, double samp_rate, double start_time, double delay_time, bool tpdu_mode);
      ~@IMPL_NAME@();

      // public callback setters
      void set_trigger_key(pmt::pmt_t key) { d_trigger_key = key; };
      void set_arming_key(pmt::pmt_t key) { d_arming_key = key; };
      void set_time_tag_key(pmt::pmt_t key) { d_time_tag_key = key; };
      void set_message(pmt::pmt_t);
      void set_holdoff(uint32_t);
      void set_samp_rate(double);
      void set_start_time(double);
      void set_delay_time(double);
      void set_tx_limit(uint64_t);

      // message port
      void control_input(pmt::pmt_t msg);

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace pdu_utils
} // namespace gr

#endif /* @GUARD_NAME@ */
