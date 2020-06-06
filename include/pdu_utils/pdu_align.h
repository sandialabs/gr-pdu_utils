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


#ifndef INCLUDED_PDU_UTILS_PDU_ALIGN_H
#define INCLUDED_PDU_UTILS_PDU_ALIGN_H

#include <pdu_utils/api.h>
#include <pdu_utils/constants.h>
#include <gnuradio/block.h>

namespace gr {
  namespace pdu_utils {

    /*!
     * \brief Align output data to syncword
     * \ingroup pdu_utils
     *
     * PDU Align attempts to find the syncword(s) in the input
     * data stream and generate an output message with a specified alignment.
     * Symbols within the sync pattern can be included in the output by
     * specifying a negative value for the offset, while symbols after the
     * end of the sync pattern can be excluded from the output by specifying
     * a positive offset.
     *
     * The operation mode determines the output when the sync is unable to be
     * found.  If ALIGN_DROP is selected, no output message will be emitted.  If
     * ALIGN_FORWARD is selected, the input pdu will be echoed at the output.
     * If ALIGN_EMPTY is selected, the output PDU will contain the metadata
     * from the incoming pdu but the data will be empty.  The default mode of
     * operation is ALIGN_DROP.
     *
     * Note: Maximum sync word length is 64 symbols.
     *
     */
    class PDU_UTILS_API pdu_align : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<pdu_align> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of pdu_utils::pdu_align.
       *
       * /param syncwords comma delimited string of syncwords to search for
       * /param threshold number of errors to allow when searching for sync
       * /param offset number of symbols before (-) or after (+) to begin output
       *               data
       * /param mode Mode to operate when sync is not found
       *               ALIGN_DROP (default) - drop pdu
       *               ALIGN_FORWARD - forward pdu
       *               ALIGN_EMPTY - emit empty pdu
       */
      static sptr make(std::string syncwords, int threshold, int offset,
        align_modes mode = ALIGN_DROP);
    };

  } // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_ALIGN_H */
