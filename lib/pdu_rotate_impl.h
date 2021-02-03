/* -*- c++ -*- */
/*
 * Copyright 2021 gr-pdu_utils author.
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

#ifndef INCLUDED_PDU_UTILS_PDU_ROTATE_IMPL_H
#define INCLUDED_PDU_UTILS_PDU_ROTATE_IMPL_H

#include <pdu_utils/pdu_rotate.h>
#include <gnuradio/blocks/rotator.h>

namespace gr {
  namespace pdu_utils {

    class pdu_rotate_impl : public pdu_rotate
    {
     private:
      blocks::rotator d_r;
      double d_phase_inc;

     public:
      pdu_rotate_impl(double phase_inc);
      ~pdu_rotate_impl();

      void msg_handler(pmt::pmt_t pdu);
      void set_phase_inc(double phase_inc);

      int work(
              int noutput_items,
              gr_vector_const_void_star &input_items,
              gr_vector_void_star &output_items
      );
    };

  } // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_ROTATE_IMPL_H */

