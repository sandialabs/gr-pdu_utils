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
#include <gnuradio/pdu_utils/api.h>

namespace gr {
namespace pdu_utils {

/*!
 * \brief PDU data to file
 * \ingroup pdu_utils
 *
 * The PDU Logger block dumps a u8, c32, or f32 data vector from PDUs to file.
 * The block lets the user specify the filename prefix, as well as how many
 * PDU data vectors are printed to each file. Note that there is no delimiter
 * between data vectors printed to the same file.
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
     *
     * @param pdus_per_file - how many PDUs to print to a file
     */
    static sptr make(std::string logfile, uint32_t pdus_per_file);

    /*!
     * \brief Return a shared_ptr to a new instance of pdu_utils::pdu_logger,
     *  where pdus_per_file is equal to 1.
     *
     * @param logfile - Filename to write to
     */
    static sptr make(std::string logfile);
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_LOGGER_H */
