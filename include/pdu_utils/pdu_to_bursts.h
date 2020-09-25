/* -*- c++ -*- */
/*
 * Copyright 2018, 2019, 2020 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_PDU_UTILS_PDU_TO_BURSTS_H
#define INCLUDED_PDU_UTILS_PDU_TO_BURSTS_H

#include <gnuradio/sync_block.h>
#include <pdu_utils/api.h>

namespace gr {
namespace pdu_utils {

/*!
 * \brief Emit a PDU data as a stream.
 *
 * The PDU to Bursts block takes data stored in the uniform vector of a PDU
 * and emits it with UHD style tx_sob and tx_eob tags. If desired, the
 * metadata dictionary can contain a tx_time tag which will append a UHD
 * style tx_time tag to the tx_sob sample causing transmission at a well
 * defined point in time.
 *
 * \ingroup pdu_utils
 *
 */
template <class T>
class PDU_UTILS_API pdu_to_bursts : virtual public gr::sync_block
{
public:
    typedef boost::shared_ptr<pdu_to_bursts<T>> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of pdu_utils::pdu_to_bursts_b.
     *
     * @param early_burst_behavior - EARLY_BURST_BEHAVIOR__APPEND,
     * EARLY_BURST_BEHAVIOR__DROP
     * @param max_queue_size - max number of PDUs to queue
     */
    static sptr make(uint32_t early_burst_behavior, uint32_t max_queue_size = 64);

    /**
     * Set Max Queue size
     *
     * @param size -
     */
    virtual void set_max_queue_size(uint32_t size) = 0;
};

typedef pdu_to_bursts<unsigned char> pdu_to_bursts_b;
typedef pdu_to_bursts<short> pdu_to_bursts_s;
typedef pdu_to_bursts<float> pdu_to_bursts_f;
typedef pdu_to_bursts<gr_complex> pdu_to_bursts_c;
} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_TO_BURSTS_H */
