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

#ifndef INCLUDED_PDU_UTILS_SANDIA_MESSAGE_DEBUG_H
#define INCLUDED_PDU_UTILS_SANDIA_MESSAGE_DEBUG_H

#include <pdu_utils/api.h>
#include <gnuradio/block.h>

namespace gr
{
  namespace pdu_utils
  {

    /*!
     * \brief Provides STDOUT message logging capabilities
     * \ingroup pdu_utils
     *
     * Sandia Message Debug provides several ports for printing messages to STDOUT
     */
    class PDU_UTILS_API sandia_message_debug : virtual public gr::block
    {
      public:
      typedef boost::shared_ptr<sandia_message_debug> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of pdu_utils::sandia_message_debug.
       *
       * To avoid accidental use of raw pointers, pdu_utils::sandia_message_debug's
       * constructor is in a private implementation
       * class. pdu_utils::sandia_message_debug::make is the public interface for
       * creating new instances.
       */
      static sptr make();

      /**
       * Returns the number of messages stored
       *
       * @return int - number of messages stored
       */
      virtual int num_messages() = 0;

      /**
       * Returns a specific stored message
       *
       * @param i - message index
       * @return pmt::pmt_t - stored message
       */
      virtual pmt::pmt_t get_message(int i) = 0;
    };

  } // namespace pdu_utils
}// namespace gr

#endif /* INCLUDED_PDU_UTILS_SANDIA_MESSAGE_DEBUG_H */

