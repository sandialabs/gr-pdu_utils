/* -*- c++ -*- */
/*
 * Copyright 2018, 2019, 2020 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_PDU_UTILS_TAKE_SKIP_TO_PDU_H
#define INCLUDED_PDU_UTILS_TAKE_SKIP_TO_PDU_H

#include <gnuradio/sync_block.h>
#include <pdu_utils/api.h>

namespace gr {
namespace pdu_utils {

#ifndef MAXIMUM_PDU_SIZE
#define MAXIMUM_PDU_SIZE
static const uint32_t TAKESKIP_MAXIMUM_PDU_SIZE = 8192;
#endif

/*!
 * \brief <+description of block+>
 * \ingroup pdu_utils
 *
 * This block will generate PDUs of size TAKE skipping SKIP samples between
 *
 */
template <class T>
class PDU_UTILS_API take_skip_to_pdu : virtual public gr::sync_block
{
public:
    typedef boost::shared_ptr<take_skip_to_pdu<T>> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of pdu_utils::take_skip_to_pdu.
     *
     * @param take - size of PDUs to generate
     * @param skip - number of samples to skip between takes
     */
    static sptr make(uint32_t take, uint32_t skip);

    virtual void set_take(uint32_t take) = 0;
    virtual void set_skip(uint32_t skip) = 0;
};

typedef take_skip_to_pdu<unsigned char> take_skip_to_pdu_b;
typedef take_skip_to_pdu<short> take_skip_to_pdu_s;
typedef take_skip_to_pdu<float> take_skip_to_pdu_f;
typedef take_skip_to_pdu<gr_complex> take_skip_to_pdu_c;
} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_TAKE_SKIP_TO_PDU_H */
