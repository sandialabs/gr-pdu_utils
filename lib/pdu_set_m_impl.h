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


#ifndef INCLUDED_PDU_UTILS_PDU_SET_M_IMPL_H
#define INCLUDED_PDU_UTILS_PDU_SET_M_IMPL_H

#include <pdu_utils/pdu_set_m.h>
#include <pdu_utils/constants.h>

namespace gr {
  namespace pdu_utils {

    class pdu_set_m_impl : public pdu_set_m
    {
     private:
      pmt::pmt_t d_k;
      pmt::pmt_t d_v;

     public:
      pdu_set_m_impl(pmt::pmt_t k, pmt::pmt_t v);
      ~pdu_set_m_impl();
      void setup_rpc(); 	// enable controlport

      void handle_msg(pmt::pmt_t msg);
      void handle_ctrl_msg(pmt::pmt_t msg);

      // getters
      pmt::pmt_t key() { return d_k; }
      pmt::pmt_t val() { return d_v; }

      // setters
      void set_key(pmt::pmt_t k);
      void set_val(pmt::pmt_t v);
      void set_kv(pmt::pmt_t k, pmt::pmt_t v);

    };

  } // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_SET_M_IMPL_H */
