/* -*- c++ -*- */
/*
 * Copyright 2018-2021 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "pdu_logger_impl.h"
#include <gnuradio/io_signature.h>
#include <cerrno>

namespace gr {
namespace pdu_utils {

pdu_logger::sptr pdu_logger::make(std::string logfile, uint32_t pdus_per_file)
{
    return gnuradio::make_block_sptr<pdu_logger_impl>(logfile, pdus_per_file);
}

pdu_logger::sptr pdu_logger::make(std::string logfile)
{
    return gnuradio::make_block_sptr<pdu_logger_impl>(logfile);
}

/*
 * The private constructor
 */
pdu_logger_impl::pdu_logger_impl(std::string logfile, uint32_t pdus_per_file)
    : gr::block(
          "pdu_logger", gr::io_signature::make(0, 0, 0), gr::io_signature::make(0, 0, 0)),
      d_logfile(logfile),
      d_pdus_per_file(pdus_per_file),
      d_burstnum(0),
      d_filenum(0)
{
    message_port_register_in(PMTCONSTSTR__pdu_in());
    set_msg_handler(PMTCONSTSTR__pdu_in(),
                    [this](pmt::pmt_t msg) { this->handle_pdu(msg); });
}

pdu_logger_impl::pdu_logger_impl(std::string logfile)
    : gr::block(
          "pdu_logger", gr::io_signature::make(0, 0, 0), gr::io_signature::make(0, 0, 0)),
      d_logfile(logfile),
      d_pdus_per_file(1),
      d_burstnum(0),
      d_filenum(0)
{
    message_port_register_in(PMTCONSTSTR__pdu_in());
    set_msg_handler(PMTCONSTSTR__pdu_in(),
                    [this](pmt::pmt_t msg) { this->handle_pdu(msg); });
}

/*
 * Our virtual destructor.
 */
pdu_logger_impl::~pdu_logger_impl() {}

void pdu_logger_impl::handle_pdu(pmt::pmt_t pdu)
{
    // make sure PDU data is formed properly
    if (!(pmt::is_pdu(pdu))) {
        GR_LOG_NOTICE(d_logger, "Received non PDU type message. Dropping");
        return;
    }

    pmt::pmt_t samples = pmt::cdr(pdu);
    size_t N = pmt::length(samples);

    if (pmt::is_c32vector(samples)) {
        const gr_complex* burst = (const gr_complex*)pmt::c32vector_elements(samples, N);
        write_data_c32(burst, N, (char*)"c32", d_filenum);
    } else if (pmt::is_f32vector(samples)) {
        const float* burst = (const float*)pmt::f32vector_elements(samples, N);
        write_data_f32(burst, N, (char*)"f32", d_filenum);
    } else if (pmt::is_u8vector(samples)) {
        const char* burst = (const char*)pmt::u8vector_elements(samples, N);
        write_data_u8(burst, N, (char*)"u8", d_filenum);
    } else {
        GR_LOG_NOTICE(d_logger, "Received PDU of unhandled data type (C32, F32, U8 supported). Dropping.");
        return;
    }
    if (++d_burstnum == d_pdus_per_file) {
        d_burstnum = 0;
        d_filenum++;
    }
}

void pdu_logger_impl::write_data_c32(const gr_complex* data,
                                   size_t len,
                                   char* name,
                                   int num)
{
    char filename[512];
    std::string str_fopen;
    if (d_burstnum == 0) {
        str_fopen = "wb";
    }
    else {
        str_fopen = "ab";
    }
    sprintf(filename, "%s%s_%04d.fc32", d_logfile.c_str(), name, num);
    FILE* fp = fopen(filename, str_fopen.c_str());
    if (fp == NULL) {
        char msg[128] = "Error opening ";
        perror(strcat(msg, filename));
    } else {
        fwrite(data, sizeof(gr_complex), len, fp);
        fclose(fp);
    }
}

void pdu_logger_impl::write_data_f32(const float* data, size_t len, char* name, int num)
{
    char filename[512];
    std::string str_fopen;
    if (d_burstnum == 0) {
        str_fopen = "wb";
    }
    else {
        str_fopen = "ab";
    }
    sprintf(filename, "%s%s_%04d.f32", d_logfile.c_str(), name, num);
    FILE* fp = fopen(filename, str_fopen.c_str());
    if (fp == NULL) {
        char msg[128] = "Error opening ";
        perror(strcat(msg, filename));
    } else {
        fwrite(data, sizeof(float), len, fp);
        fclose(fp);
    }
}

void pdu_logger_impl::write_data_u8(const char* data, size_t len, char* name, int num)
{
    char filename[512];
    std::string str_fopen;
    if (d_burstnum == 0) {
        str_fopen = "wb";
    }
    else {
        str_fopen = "ab";
    }
    sprintf(filename, "%s%s_%04d.u8", d_logfile.c_str(), name, num);
    FILE* fp = fopen(filename, str_fopen.c_str());
    if (fp == NULL) {
        char msg[128] = "Error opening ";
        perror(strcat(msg, filename));
    } else {
        fwrite(data, sizeof(char), len, fp);
        fclose(fp);
    }
}

} /* namespace pdu_utils */
} /* namespace gr */
