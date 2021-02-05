/* -*- c++ -*- */
/*
 * Copyright 2018-2021 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_PDU_UTILS_PDU_LOGGER_H
#define INCLUDED_PDU_UTILS_PDU_LOGGER_H

#include <gnuradio/sync_block.h>
#include <pdu_utils/api.h>

namespace gr {
namespace pdu_utils {

/*!
 * \brief PDU data to file
 * \ingroup pdu_utils
 *
 * The PDU Logger block dumps a c32 or f32 data vector from PDUs to file.
 *
 *
 */
class PDU_UTILS_API pdu_logger : virtual public gr::block
{
public:
    typedef std::shared_ptr<pdu_logger> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of pdu_utils::pdu_logger.
     *
     * @param logfile - Filename to write to
     */
    static sptr make(std::string logfile);
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_LOGGER_H */
