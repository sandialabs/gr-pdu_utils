/* -*- c++ -*- */
/*
 * Copyright 2018, 2019, 2020 National Technology & Engineering Solutions of Sa>
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Gover>
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "pdu_slice_impl.h"
#include "gnuradio/pdu_utils/constants.h"

namespace gr {
namespace pdu_utils {

typename pdu_slice::sptr
pdu_slice::make(std::string slice)
{
    return gnuradio::make_block_sptr<pdu_slice_impl>(slice);
}

/*
 * The private constructor
 */
pdu_slice_impl::pdu_slice_impl(std::string slice)
    : gr::block("pdu_slice",
                gr::io_signature::make(0, 0, 0),
                gr::io_signature::make(0, 0, 0)),
      d_slice(slice),
      d_start_index(0),
      d_stop_index(INT_MAX),
      d_stride_size(1),
      d_blank_start(false),
      d_blank_stop(false)
{
    message_port_register_in(PMTCONSTSTR__pdu_in());
    set_msg_handler(PMTCONSTSTR__pdu_in(),
                    [this](pmt::pmt_t msg) { this->pdu_slice_impl::handle_pdu(msg); });
    message_port_register_out(PMTCONSTSTR__pdu_out());
}

/*
 * Our virtual destructor.
 */
pdu_slice_impl::~pdu_slice_impl() {}


template<typename T>
void pdu_slice_impl::slice_vector(std::vector<T> &in_data, std::vector<T> &out_data)
{
  int v_len = in_data.size();
  out_data.reserve(v_len);

  // update index values according to length of PDU data
  if (d_start_index < -v_len) d_start_index = 0;
  if (d_start_index < 0) d_start_index += v_len;
  if (d_start_index > v_len) d_start_index = (d_stride_size > 0) ? v_len : v_len-1;

  if (d_stop_index < -v_len) d_stop_index = 0;
  if (d_stop_index < 0) d_stop_index += v_len;
  if (d_stop_index > v_len) d_stop_index = v_len;

  // if slice notation has blank values, update the indices now
  // positive stride puts the start of the slice at the front
  // negative stride puts the start of the slice at the back
  if (d_blank_start == true) d_start_index = (d_stride_size > 0) ? 0 : v_len-1;
  if (d_blank_stop == true) d_stop_index = (d_stride_size > 0) ? v_len : -1;

  // positive stride - forward traversal
  if (d_stride_size > 0) {
     for (int i = d_start_index; i < d_stop_index; i += d_stride_size) {
         out_data.push_back(in_data[i]);
      }
  } else {
  // negative stride - backward traversal
     for (int i = d_start_index; i > d_stop_index; i += d_stride_size) {
          out_data.push_back(in_data[i]);
     }
  }
}


void pdu_slice_impl::handle_pdu(pmt::pmt_t pdu)
{
    // make sure PDU data is formed properly
    if (!(pmt::is_pair(pdu))) {
        GR_LOG_NOTICE(d_logger, "received unexpected PMT (non-pair)");
        return;
    }

    // parse slice notation and update values
    try {
        this->set_slice(d_slice);
    }
    catch (const std::exception&) {
        throw std::runtime_error("");
    }
    pmt::pmt_t meta = pmt::car(pdu);
    pmt::pmt_t v_data = pmt::cdr(pdu);

    // is data u8int?
    if (pmt::is_u8vector(v_data)) {
        std::vector<uint8_t> in_data = pmt::u8vector_elements(v_data);
        std::vector<uint8_t> out_data;
        slice_vector(in_data, out_data);

        // publish the new pdu
        message_port_pub(
            PMTCONSTSTR__pdu_out(),
            (pmt::cons(meta, pmt::init_u8vector(out_data.size(), out_data))));

    }
    // is data float?
    else if (pmt::is_f32vector(v_data)) {
        std::vector<float> in_data = pmt::f32vector_elements(v_data);
        std::vector<float> out_data;
        slice_vector(in_data, out_data);

        // publish the new pdu
        message_port_pub(
            PMTCONSTSTR__pdu_out(),
            (pmt::cons(meta, pmt::init_f32vector(out_data.size(), out_data))));

    }
    // is data complex32?
    else if (pmt::is_c32vector(v_data)) {
        std::vector<gr_complex> in_data = pmt::c32vector_elements(v_data);
        std::vector<gr_complex> out_data;
        slice_vector(in_data, out_data);

        // publish the new pdu
        message_port_pub(
            PMTCONSTSTR__pdu_out(),
            (pmt::cons(meta, pmt::init_c32vector(out_data.size(), out_data))));
    }
    else {
        GR_LOG_WARN(d_logger, "Got unknown PDU vector type, dropping");
    }

}

void pdu_slice_impl::set_slice(std::string slice)
{
    d_slice = slice;
    std::string start_token, stop_token, stride_token, tmp;

    // Make sure slice notation begins and ends in brackets
    if (d_slice.find("[") !=0 ||
        d_slice.find("]") != d_slice.size()-1) {
            GR_LOG_ERROR(d_logger, "received invalid slice notation");
            throw std::runtime_error("");
    }

    // Find number of colons to determine how to parse notation
    size_t num_colon = count(d_slice.begin(), d_slice.end(), ':');

    switch (num_colon) {
        case 1 :    // one colon - [start index : stop index] with stride size = 1
            // parse from open bracket to colon for start index
            start_token = d_slice.substr(1,d_slice.find(":")-1);
            // if blank, set boolean to true
            // otherwise, try to interpret as integer
            if (!start_token.compare("")) {
                d_blank_start = true;
            } else {
                d_blank_start = false;
                try {
                    d_start_index = stoi(start_token);
                }
                catch (const std::exception&) {
                    GR_LOG_ERROR(d_logger, "received invalid start index for slice");
                    throw std::runtime_error("");
                }
            }
            // parse from colon to end bracket for stop index
            tmp = d_slice.substr(d_slice.find(":")+1);
            stop_token = tmp.substr(0,tmp.size()-1);
            // if blank, set boolean to true
            // otherwise, try to interpret as integer
            if (!stop_token.compare("")) {
                d_blank_stop = true;
            } else {
                d_blank_stop = false;
                try {
                    d_stop_index = stoi(stop_token);
                }
                catch (const std::exception&) {
                    GR_LOG_ERROR(d_logger, "received invalid stop index for slice");
                    throw std::runtime_error("");
                }
                // set stride size to default
                d_stride_size = 1;
            }
            break;
        case 2 :     // two colons - [start index : stop index : stride size]
            // parse from open bracket to first colon for start index
            start_token = d_slice.substr(1,d_slice.find(":")-1);
            // if blank, set boolean to true
            // otherwise, try to interpret as integer
            if (!start_token.compare("")) {
                d_blank_start = true;
            } else {
                d_blank_start = false;
                try {
                    d_start_index = stoi(start_token);
                }
                catch (const std::exception&) {
                    GR_LOG_ERROR(d_logger, "received invalid start index for slice");
                    throw std::runtime_error("");
                }
            }
            // parse from first colon to second colon for stop index
            tmp = d_slice.substr(d_slice.find(":")+1);
            stop_token = tmp.substr(0,tmp.find(":"));
            // if blank, set boolean to true
            // otherwise, try to interpret as integer
            if (!stop_token.compare("")) {
                d_blank_stop = true;
            } else {
                d_blank_stop = false;
                try {
                    d_stop_index = stoi(stop_token);
                }
                catch (const std::exception&) {
                    GR_LOG_ERROR(d_logger, "received invalid stop index for slice");
                    throw std::runtime_error("");
                }
            }
            // parse from second colon to end bracket for stride size
            tmp = tmp.substr(tmp.find(":")+1);
            stride_token = tmp.substr(0,tmp.size()-1);
            // if blank, set stride size to 1
            // otherwise, try to interpret as integer
            if (!stride_token.compare("")) {
                d_stride_size = 1;
            } else {
                try {
                    d_stride_size = stoi(stride_token);
                }
                catch (const std::exception&) {
                    GR_LOG_ERROR(d_logger, "received invalid stride size for slice");
                    throw std::runtime_error("");
                }
                // stride size cannot be 0
                if (d_stride_size == 0) {
                    GR_LOG_ERROR(d_logger, "received zero stride size for slice");
                    throw std::runtime_error("");
            }
            break;
        // if number of colons is not 1 or 2, notation is invalid
        default :
                GR_LOG_ERROR(d_logger, "received invalid slice notation");
                throw std::runtime_error("");
        }
    }
}


} /* namespace pdu_utils */
} /* namespace gr */
