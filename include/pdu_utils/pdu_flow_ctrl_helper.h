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


#ifndef INCLUDED_PDU_UTILS_PDU_FLOW_CTRL_HELPER_H
#define INCLUDED_PDU_UTILS_PDU_FLOW_CTRL_HELPER_H

#include <pdu_utils/api.h>
#include <gnuradio/basic_block.h>

namespace gr {
  namespace pdu_utils {

    typedef boost::weak_ptr<gr::basic_block> basic_block_wptr;

    /*!
     * \brief Convenience functions for watching the number of messages waiting
     * at the given blocks' inputs.
     */
    class PDU_UTILS_API pdu_flow_ctrl_helper
    {
    public:
      pdu_flow_ctrl_helper(std::vector<gr::basic_block_sptr> blocks);
      ~pdu_flow_ctrl_helper();

      int max_nmsgs();
      void print_nmsgs();
    private:
      struct block_port {
        basic_block_wptr block;
        std::vector<pmt::pmt_t> ports;
      };

      std::vector<block_port> d_blocks;
      const pmt::pmt_t SYSTEM_PORT = pmt::mp("system");
    };

  } // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_FLOW_CTRL_HELPER_H */

