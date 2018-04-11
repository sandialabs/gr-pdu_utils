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

#ifndef INCLUDED_PDU_UTILS_UPSAMPLE_IMPL_H
#define INCLUDED_PDU_UTILS_UPSAMPLE_IMPL_H

#include <pdu_utils/upsample.h>
#include <pdu_utils/constants.h>

namespace gr {
  namespace pdu_utils {

    class upsample_impl : public upsample
    {
     private:
      uint32_t d_n;
      bool d_repeat;

     public:
      upsample_impl(uint32_t n, bool repeat);
      ~upsample_impl();

      void handle_msg(pmt::pmt_t);

      void set_n(uint32_t n);
      void set_repeat(bool repeat);
    };

  } // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_UPSAMPLE_IMPL_H */
