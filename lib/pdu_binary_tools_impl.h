/* -*- c++ -*- */
/* 
 * Copyright 2018 gr-pdu_utils author.
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

#ifndef INCLUDED_PDU_UTILS_PDU_BINARY_TOOLS_IMPL_H
#define INCLUDED_PDU_UTILS_PDU_BINARY_TOOLS_IMPL_H

#include <pdu_utils/pdu_binary_tools.h>

namespace gr {
  namespace pdu_utils {

    class pdu_binary_tools_impl : public pdu_binary_tools
    {
     private:
      void handle_msg_bit_flip(pmt::pmt_t pdu);
      void handle_msg_to_nrz(pmt::pmt_t pdu);
      void handle_msg_from_nrz(pmt::pmt_t pdu);
      void handle_msg_slice(pmt::pmt_t pdu);
      void handle_msg_endian8(pmt::pmt_t pdu);
      void handle_msg_passthrough(pmt::pmt_t pdu);


     public:
      pdu_binary_tools_impl(uint8_t mode);
      ~pdu_binary_tools_impl();

      // Enum for mode setting in this block
      enum Modes{BIT_FLIP=0, TO_NRZ, FROM_NRZ, SLICE, ENDIAN_SWAP8};

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_BINARY_TOOLS_IMPL_H */

