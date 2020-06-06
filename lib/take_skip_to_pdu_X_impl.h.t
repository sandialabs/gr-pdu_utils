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
      std::vector<@I_TYPE@> d_vector;
      uint32_t d_take;
      uint32_t d_skip;
      uint64_t d_next;
      uint64_t d_burst_counter;
      bool d_triggered;
      uint8_t d_prev_byte;
      pmt::pmt_t d_meta_dict;

      void publish_message(void);

     public:
      @IMPL_NAME@(uint32_t take, uint32_t skip);
      ~@IMPL_NAME@();

      void set_take(uint32_t take);
      void set_skip(uint32_t skip);

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace pdu_utils
} // namespace gr

#endif /* @GUARD_NAME@ */
