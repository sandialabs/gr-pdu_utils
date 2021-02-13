/* -*- c++ -*- */
/*
 * Copyright 2018-2021 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_PDU_UTILS_PDU_LOGGER_IMPL_H
#define INCLUDED_PDU_UTILS_PDU_LOGGER_IMPL_H

#include <pdu_utils/constants.h>
#include <pdu_utils/pdu_logger.h>

namespace gr {
namespace pdu_utils {

class pdu_logger_impl : public pdu_logger
{
private:
    std::string d_logfile;
    int d_burstnum;

    void write_data_c32(const gr_complex* data, size_t len, char* name, int num);
    void write_data_f32(const float* data, size_t len, char* name, int num);
    void write_data_u8(const char* data, size_t len, char* name, int num);

    void handle_pdu(pmt::pmt_t pdu);

public:
    pdu_logger_impl(std::string logfile);

    ~pdu_logger_impl() override;
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_LOGGER_IMPL_H */
