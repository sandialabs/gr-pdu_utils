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

#ifndef INCLUDED_PDU_UTILS_MESSAGE_GATE_IMPL_H
#define INCLUDED_PDU_UTILS_MESSAGE_GATE_IMPL_H

#include <pdu_utils/message_gate.h>
#include <pdu_utils/constants.h>

namespace gr {
  namespace pdu_utils {

    class message_gate_impl : public message_gate
    {
     private:
      bool    d_enabled;
      uint64_t d_n_passed;
      uint64_t d_n_blocked;

     public:
      message_gate_impl(bool enabled);
      ~message_gate_impl();
      bool stop();        // overloaded for print output
      void setup_rpc(); 	// enable controlport

      void handle_msg(pmt::pmt_t msg);

      void reset(void);
      void set_enabled(bool enabled);
      bool get_enabled(void) { return d_enabled; };
      uint64_t get_n_passed(void) { return d_n_passed; };
      uint64_t get_n_blocked(void) { return d_n_blocked; };

    };

  } // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_MESSAGE_GATE_IMPL_H */
