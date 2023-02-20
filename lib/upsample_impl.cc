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

#include "upsample_impl.h"
#include <gnuradio/io_signature.h>
#include <boost/format.hpp>
namespace gr {
namespace pdu_utils {

upsample::sptr upsample::make(uint32_t n, bool repeat)
{
    return gnuradio::make_block_sptr<upsample_impl>(n, repeat);
}

/*
 * The private constructor
 */
upsample_impl::upsample_impl(uint32_t n, bool repeat)
    : gr::block("upsample", io_signature::make(0, 0, 0), io_signature::make(0, 0, 0)),
      d_repeat(repeat)
{
    if (n > 0) {
        d_n = n;
    } else {
        throw std::invalid_argument("pdu_utils upsample: ERROR! n must be >= 1!\n");
    }
    message_port_register_in(PMTCONSTSTR__pdu_in());
    set_msg_handler(PMTCONSTSTR__pdu_in(),
                    [this](pmt::pmt_t msg) { this->handle_msg(msg); });
    message_port_register_out(PMTCONSTSTR__pdu_out());
}

/*
 * Our virtual destructor.
 */
upsample_impl::~upsample_impl() {}


void upsample_impl::handle_msg(pmt::pmt_t pdu)
{
    // make sure PDU data is formed properly
    if (!(pmt::is_pair(pdu))) {
        d_logger->notice("received unexpected PMT (non-pair)");
        return;
    }

    gr::thread::scoped_lock l(d_setlock);

    // if the repquested repetition is 1, just re-publish the message
    if (d_n == 1) {
        message_port_pub(PMTCONSTSTR__pdu_out(), pdu);
        return;
    }

    pmt::pmt_t meta = pmt::car(pdu);
    pmt::pmt_t v_data = pmt::cdr(pdu);

    if (!(is_dict(meta) && pmt::is_uniform_vector(v_data))) {
        d_logger->notice("received unexpected PMT (non-PDU)");
        return;
    }

    uint32_t v_len = pmt::length(v_data);
    uint32_t out_len = v_len * d_n;

    // if the input length is zero, just re-publish the message
    if (v_len == 0) {
        message_port_pub(PMTCONSTSTR__pdu_out(), pdu);
        return;

    } else if (pmt::is_u8vector(v_data)) {
        // 8 bit integers
        const std::vector<uint8_t> in_data = pmt::u8vector_elements(v_data);
        std::vector<uint8_t> output;
        if (d_repeat) {
            output.reserve(out_len);
            for (size_t ii = 0; ii < v_len; ii++) {
                for (size_t jj = 0; jj < d_n; jj++) {
                    output.push_back(in_data[ii]);
                }
            }
        } else {
            output.resize(out_len, 0);
            for (size_t ii = 0; ii < v_len; ii++) {
                output[ii * d_n] = in_data[ii];
            }
        }
        message_port_pub(PMTCONSTSTR__pdu_out(),
                         pmt::cons(meta, pmt::init_u8vector(output.size(), output)));

    } else if (pmt::is_s8vector(v_data)) {
        // 8 bit integers
        const std::vector<int8_t> in_data = pmt::s8vector_elements(v_data);
        std::vector<int8_t> output;
        if (d_repeat) {
            output.reserve(out_len);
            for (size_t ii = 0; ii < v_len; ii++) {
                for (size_t jj = 0; jj < d_n; jj++) {
                    output.push_back(in_data[ii]);
                }
            }
        } else {
            output.resize(out_len, 0);
            for (size_t ii = 0; ii < v_len; ii++) {
                output[ii * d_n] = in_data[ii];
            }
        }
        message_port_pub(PMTCONSTSTR__pdu_out(),
                         pmt::cons(meta, pmt::init_s8vector(output.size(), output)));

    } else if (pmt::is_u16vector(v_data)) {
        // 16 bit integers
        const std::vector<uint16_t> in_data = pmt::u16vector_elements(v_data);
        std::vector<uint16_t> output;
        if (d_repeat) {
            output.reserve(out_len);
            for (size_t ii = 0; ii < v_len; ii++) {
                for (size_t jj = 0; jj < d_n; jj++) {
                    output.push_back(in_data[ii]);
                }
            }
        } else {
            output.resize(out_len, 0);
            for (size_t ii = 0; ii < v_len; ii++) {
                output[ii * d_n] = in_data[ii];
            }
        }
        message_port_pub(PMTCONSTSTR__pdu_out(),
                         pmt::cons(meta, pmt::init_u16vector(output.size(), output)));

    } else if (pmt::is_s16vector(v_data)) {
        // 16 bit integers
        const std::vector<int16_t> in_data = pmt::s16vector_elements(v_data);
        std::vector<int16_t> output;
        if (d_repeat) {
            output.reserve(out_len);
            for (size_t ii = 0; ii < v_len; ii++) {
                for (size_t jj = 0; jj < d_n; jj++) {
                    output.push_back(in_data[ii]);
                }
            }
        } else {
            output.resize(out_len, 0);
            for (size_t ii = 0; ii < v_len; ii++) {
                output[ii * d_n] = in_data[ii];
            }
        }
        message_port_pub(PMTCONSTSTR__pdu_out(),
                         pmt::cons(meta, pmt::init_s16vector(output.size(), output)));

    } else if (pmt::is_u32vector(v_data)) {
        // 32 bit integers
        const std::vector<uint32_t> in_data = pmt::u32vector_elements(v_data);
        std::vector<uint32_t> output;
        if (d_repeat) {
            output.reserve(out_len);
            for (size_t ii = 0; ii < v_len; ii++) {
                for (size_t jj = 0; jj < d_n; jj++) {
                    output.push_back(in_data[ii]);
                }
            }
        } else {
            output.resize(out_len, 0);
            for (size_t ii = 0; ii < v_len; ii++) {
                output[ii * d_n] = in_data[ii];
            }
        }
        message_port_pub(PMTCONSTSTR__pdu_out(),
                         pmt::cons(meta, pmt::init_u32vector(output.size(), output)));

    } else if (pmt::is_s32vector(v_data)) {
        // 32 bit integers
        const std::vector<int32_t> in_data = pmt::s32vector_elements(v_data);
        std::vector<int32_t> output;
        if (d_repeat) {
            output.reserve(out_len);
            for (size_t ii = 0; ii < v_len; ii++) {
                for (size_t jj = 0; jj < d_n; jj++) {
                    output.push_back(in_data[ii]);
                }
            }
        } else {
            output.resize(out_len, 0);
            for (size_t ii = 0; ii < v_len; ii++) {
                output[ii * d_n] = in_data[ii];
            }
        }
        message_port_pub(PMTCONSTSTR__pdu_out(),
                         pmt::cons(meta, pmt::init_s32vector(output.size(), output)));

    } else if (pmt::is_u64vector(v_data) || pmt::is_s64vector(v_data)) {
        // 64 bit integers
        const std::vector<uint64_t> in_data = pmt::u64vector_elements(v_data);
        std::vector<uint64_t> output;
        if (d_repeat) {
            output.reserve(out_len);
            for (size_t ii = 0; ii < v_len; ii++) {
                for (size_t jj = 0; jj < d_n; jj++) {
                    output.push_back(in_data[ii]);
                }
            }
        } else {
            output.resize(out_len, 0);
            for (size_t ii = 0; ii < v_len; ii++) {
                output[ii * d_n] = in_data[ii];
            }
        }
        message_port_pub(PMTCONSTSTR__pdu_out(),
                         pmt::cons(meta, pmt::init_u64vector(output.size(), output)));

    } else if (pmt::is_s64vector(v_data)) {
        // 64 bit integers
        const std::vector<int64_t> in_data = pmt::s64vector_elements(v_data);
        std::vector<int64_t> output;
        if (d_repeat) {
            output.reserve(out_len);
            for (size_t ii = 0; ii < v_len; ii++) {
                for (size_t jj = 0; jj < d_n; jj++) {
                    output.push_back(in_data[ii]);
                }
            }
        } else {
            output.resize(out_len, 0);
            for (size_t ii = 0; ii < v_len; ii++) {
                output[ii * d_n] = in_data[ii];
            }
        }
        message_port_pub(PMTCONSTSTR__pdu_out(),
                         pmt::cons(meta, pmt::init_s64vector(output.size(), output)));

    } else if (pmt::is_f32vector(v_data)) {
        // single precision floating point
        const std::vector<float> in_data = pmt::f32vector_elements(v_data);
        std::vector<float> output;
        if (d_repeat) {
            output.reserve(out_len);
            for (size_t ii = 0; ii < v_len; ii++) {
                for (size_t jj = 0; jj < d_n; jj++) {
                    output.push_back(in_data[ii]);
                }
            }
        } else {
            output.resize(out_len, 0);
            for (size_t ii = 0; ii < v_len; ii++) {
                output[ii * d_n] = in_data[ii];
            }
        }
        message_port_pub(PMTCONSTSTR__pdu_out(),
                         pmt::cons(meta, pmt::init_f32vector(output.size(), output)));

    } else if (pmt::is_f64vector(v_data)) {
        // double precision floating point
        const std::vector<double> in_data = pmt::f64vector_elements(v_data);
        std::vector<double> output;
        if (d_repeat) {
            output.reserve(out_len);
            for (size_t ii = 0; ii < v_len; ii++) {
                for (size_t jj = 0; jj < d_n; jj++) {
                    output.push_back(in_data[ii]);
                }
            }
        } else {
            output.resize(out_len, 0);
            for (size_t ii = 0; ii < v_len; ii++) {
                output[ii * d_n] = in_data[ii];
            }
        }
        message_port_pub(PMTCONSTSTR__pdu_out(),
                         pmt::cons(meta, pmt::init_f64vector(output.size(), output)));

    } else if (pmt::is_c32vector(v_data)) {
        // gr_complex
        const std::vector<gr_complex> in_data = pmt::c32vector_elements(v_data);
        std::vector<gr_complex> output;
        if (d_repeat) {
            output.reserve(out_len);
            for (size_t ii = 0; ii < v_len; ii++) {
                for (size_t jj = 0; jj < d_n; jj++) {
                    output.push_back(in_data[ii]);
                }
            }
        } else {
            output.resize(out_len, 0);
            for (size_t ii = 0; ii < v_len; ii++) {
                output[ii * d_n] = in_data[ii];
            }
        }
        message_port_pub(PMTCONSTSTR__pdu_out(),
                         pmt::cons(meta, pmt::init_c32vector(output.size(), output)));
    } else if (pmt::is_c64vector(v_data)) {
        // TODO: support this...
        d_logger->notice("PDU of type complex double not supported, dropped");
        return;

    } else {
        // drop message and return
        d_logger->notice("unknown PDU vector type, dropped");
        return;
    }
    return;
}


void upsample_impl::set_n(uint32_t n)
{
    gr::thread::scoped_lock l(d_setlock);

    if (n > 0) {
        d_n = n;
    }
}


void upsample_impl::set_repeat(bool repeat)
{
    gr::thread::scoped_lock l(d_setlock);

    d_repeat = repeat;
}

} /* namespace pdu_utils */
} /* namespace gr */
