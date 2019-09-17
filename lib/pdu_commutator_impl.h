/* -*- c++ -*- */
/* 
 * Copyright 2019 gr-pdu_utils author.
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

#ifndef INCLUDED_PDU_UTILS_PDU_COMMUTATOR_IMPL_H
#define INCLUDED_PDU_UTILS_PDU_COMMUTATOR_IMPL_H

#include <pdu_utils/pdu_commutator.h>
#include <pdu_utils/constants.h>

namespace gr {
  namespace pdu_utils {

    class pdu_commutator_impl : public pdu_commutator
    {
     private:
      size_t d_num_paths;
      size_t d_current_path;
      std::vector<pmt::pmt_t> d_output_ports;

     public:
      pdu_commutator_impl(size_t num_paths);
      ~pdu_commutator_impl();
      void handle_pdu(pmt::pmt_t pdu);

      // Where all the action really happens
      /*void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);*/
    };

  } // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_COMMUTATOR_IMPL_H */

