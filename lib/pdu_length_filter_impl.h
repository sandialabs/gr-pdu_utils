/* -*- c++ -*- */
/*
 * Copyright 2018 <+YOU OR YOUR COMPANY+>.
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

#ifndef INCLUDED_PDU_UTILS_PDU_LENGTH_FILTER_IMPL_H
#define INCLUDED_PDU_UTILS_PDU_LENGTH_FILTER_IMPL_H

#include <pdu_utils/pdu_length_filter.h>
#include <pdu_utils/constants.h>


namespace gr {
  namespace pdu_utils {

    class pdu_length_filter_impl : public pdu_length_filter
    {
     private:
      uint32_t    d_length;
      bool        d_drop_long;
      uint64_t    d_n_passed;
      uint64_t    d_n_blocked;

     public:
      pdu_length_filter_impl(uint32_t length, bool drop_long);
      ~pdu_length_filter_impl();
      bool stop();        // overloaded for print output

      void handle_pdu(pmt::pmt_t pdu);
    };

  } // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_LENGTH_FILTER_IMPL_H */
