/* -*- c++ -*- */
/*
 * Copyright 2018 National Technology & Engineering Solutions of Sandia, LLC (NTESS).
 * Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government retains
 * certain rights in this software.
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

#ifndef INCLUDED_PDU_UTILS_EXTRACT_METADATA_H
#define INCLUDED_PDU_UTILS_EXTRACT_METADATA_H

#include <gnuradio/block.h>
#include <pdu_utils/api.h>

namespace gr {
namespace pdu_utils {

/*!
 * \brief Extracts and emits a specified K-V pair
 * \ingroup pdu_utils
 *
 * This block accepts either a PDU or a dictionary and emits the specified
 * key-value pair as a message that can be used with the QT message blocks.
 *
 * For number Values, provides for scale and offset capabilities in the form y = {scale}x
 * + {offset}
 */
class PDU_UTILS_API extract_metadata : virtual public gr::block
{
public:
    typedef boost::shared_ptr<extract_metadata> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of pdu_utils::extract_metadata.
     *
     * @param key - dict key to look for
     * @param scale - scale factor to apply to number values
     * @param offset - offset to apply to number values.
     */
    static sptr make(pmt::pmt_t key, float scale, float offset);

    /**
     * Set dict key to look for
     *
     * @param key - pmt key
     */
    virtual void set_key(pmt::pmt_t key) = 0;

    /**
     * Set scale factor for number values
     *
     * @param scale - scale factor
     */
    virtual void set_scale(float scale) = 0;

    /**
     * Set Offset for number values
     *
     * @param offset - offset value
     */
    virtual void set_offset(float offset) = 0;

    /**
     * Returns current key value
     *
     * @return pmt::pmt_t
     */
    virtual pmt::pmt_t get_key(void) = 0;

    /**
     * Returns current scale value
     *
     * @return float
     */
    virtual float get_scale(void) = 0;

    /**
     * Returns current offset value
     *
     * @return float
     */
    virtual float get_offset(void) = 0;
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_EXTRACT_METADATA_H */
