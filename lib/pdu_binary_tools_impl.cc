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

#include "pdu_binary_tools_impl.h"
#include "gnuradio/pdu_utils/constants.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace pdu_utils {

pdu_binary_tools::sptr pdu_binary_tools::make(uint8_t mode)
{
    return gnuradio::make_block_sptr<pdu_binary_tools_impl>(mode);
}

/*
 * The private constructor
 */
pdu_binary_tools_impl::pdu_binary_tools_impl(uint8_t mode)
    : gr::block("pdu_binary_tools",
                     gr::io_signature::make(0, 0, 0),
                     gr::io_signature::make(0, 0, 0))
{

    // This block will handle a lot of really simply binary transforms
    // based on some enum MODE

    // TO DO: bit flip, 0/1 to -1/1, -1/1 to 0/1, slice, ...?

    // register message ports
    message_port_register_in(PMTCONSTSTR__pdu_in());
    message_port_register_out(PMTCONSTSTR__pdu_out());


    // bit flip mode
    switch (mode) {
    case pdu_binary_tools::BIT_FLIP: {
        GR_LOG_DEBUG(d_logger, "pdu binary tools in BIT FLIP mode");
        set_msg_handler(PMTCONSTSTR__pdu_in(),
                        [this](pmt::pmt_t msg) { this->handle_msg_bit_flip(msg); });
        break;
    }
    case pdu_binary_tools::TO_NRZ: {
        GR_LOG_DEBUG(d_logger, "pdu binary tools in TO NRZ mode");
        set_msg_handler(PMTCONSTSTR__pdu_in(),
                        [this](pmt::pmt_t msg) { this->handle_msg_to_nrz(msg); });
        break;
    }
    case pdu_binary_tools::FROM_NRZ: {
        GR_LOG_DEBUG(d_logger, "pdu binary tools in FROM NRZ mode");
        set_msg_handler(PMTCONSTSTR__pdu_in(),
                        [this](pmt::pmt_t msg) { this->handle_msg_from_nrz(msg); });
        break;
    }
    case pdu_binary_tools::SLICE: {
        GR_LOG_DEBUG(d_logger, "pdu binary tools in SLICE mode");
        set_msg_handler(PMTCONSTSTR__pdu_in(),
                        [this](pmt::pmt_t msg) { this->handle_msg_slice(msg); });
        break;
    }
    case pdu_binary_tools::ENDIAN_SWAP8: {
        GR_LOG_DEBUG(d_logger, "pdu binary tools in ENDIAN_SWAP8 mode");
        set_msg_handler(PMTCONSTSTR__pdu_in(),
                        [this](pmt::pmt_t msg) { this->handle_msg_endian8(msg); });
        break;
    }
    case pdu_binary_tools::MANCHESTER_ENCODE: {
        GR_LOG_DEBUG(d_logger, "pdu binary tools in MANCHESTER_ENCODE mode");
        set_msg_handler(PMTCONSTSTR__pdu_in(), [this](pmt::pmt_t msg) {
            this->handle_msg_manchester_encode(msg);
        });
        break;
    }
    case pdu_binary_tools::MANCHESTER_DECODE: {
        GR_LOG_DEBUG(d_logger, "pdu binary tools in MANCHESTER_DECODE mode");
        set_msg_handler(PMTCONSTSTR__pdu_in(), [this](pmt::pmt_t msg) {
            this->handle_msg_manchester_decode(msg);
        });
        break;
    }
    default: {
        GR_LOG_DEBUG(d_logger, "pdu binary tools in PASSTHROUGH mode");
        set_msg_handler(PMTCONSTSTR__pdu_in(),
                        [this](pmt::pmt_t msg) { this->handle_msg_passthrough(msg); });
    }
    }
}

/*
 * Our virtual destructor.
 */
pdu_binary_tools_impl::~pdu_binary_tools_impl() {}

void pdu_binary_tools_impl::handle_msg_passthrough(pmt::pmt_t pdu)
{
    message_port_pub(PMTCONSTSTR__pdu_out(), pdu);
}

///////////////////////////////////////////////////////
// Bit Flip [1,0,1] to [0,1,0]
///////////////////////////////////////////////////////
void pdu_binary_tools_impl::handle_msg_bit_flip(pmt::pmt_t pdu)
{
    // make sure PDU data is formed properly
    if (!(pmt::is_pair(pdu))) {
        GR_LOG_NOTICE(d_logger, "received unexpected PMT (non-pair)");
        return;
    }

    pmt::pmt_t meta = pmt::car(pdu);
    pmt::pmt_t v_data = pmt::cdr(pdu);

    // extract uint8 data
    if (pmt::is_u8vector(v_data)) {
        std::vector<uint8_t> data = pmt::u8vector_elements(v_data);

        // flip every bit (xor)
        std::transform(data.begin(), data.end(), data.begin(), [](uint8_t x) -> uint8_t {
            return x ^ 1;
        });

        // publish the new pdu
        message_port_pub(PMTCONSTSTR__pdu_out(),
                         (pmt::cons(meta, pmt::init_u8vector(data.size(), data))));
    } else {
        GR_LOG_WARN(d_logger, "Failed to bit flip the data because it is not a u8vector");
        message_port_pub(PMTCONSTSTR__pdu_out(), pdu);
    }
}


///////////////////////////////////////////////////////
// To NRZ [1,0,1] to [1,-1,1]
///////////////////////////////////////////////////////
void pdu_binary_tools_impl::handle_msg_to_nrz(pmt::pmt_t pdu)
{
    // make sure PDU data is formed properly
    if (!(pmt::is_pair(pdu))) {
        GR_LOG_NOTICE(d_logger, "received unexpected PMT (non-pair)");
        return;
    }

    pmt::pmt_t meta = pmt::car(pdu);
    pmt::pmt_t v_data = pmt::cdr(pdu);

    // extract data
    if (pmt::is_u8vector(v_data)) {
        std::vector<uint8_t> in_data = pmt::u8vector_elements(v_data);
        std::vector<float> out_data(in_data.size());

        // 1*2-1 = 1 and 0*2-1 = -1
        std::transform(in_data.begin(),
                       in_data.end(),
                       out_data.begin(),
                       [](uint8_t x) -> float { return (x * 2) - 1; });

        // publish the new pdu
        message_port_pub(
            PMTCONSTSTR__pdu_out(),
            (pmt::cons(meta, pmt::init_f32vector(out_data.size(), out_data))));
    } else {
        GR_LOG_WARN(d_logger, "Failed to 'to nrz' the data because it is not a u8vector");
        message_port_pub(PMTCONSTSTR__pdu_out(), pdu);
    }
}


///////////////////////////////////////////////////////
// From NRZ [1,-1,1] to [1,0,1]
///////////////////////////////////////////////////////
void pdu_binary_tools_impl::handle_msg_from_nrz(pmt::pmt_t pdu)
{
    // make sure PDU data is formed properly
    if (!(pmt::is_pair(pdu))) {
        GR_LOG_NOTICE(d_logger, "received unexpected PMT (non-pair)");
        return;
    }

    pmt::pmt_t meta = pmt::car(pdu);
    pmt::pmt_t v_data = pmt::cdr(pdu);

    // extract data
    if (pmt::is_f32vector(v_data)) {
        std::vector<float> in_data = pmt::f32vector_elements(v_data);
        std::vector<uint8_t> out_data(in_data.size());

        // (1+1)/2=1 and (-1+1)/2=0
        std::transform(in_data.begin(),
                       in_data.end(),
                       out_data.begin(),
                       [](float x) -> uint8_t { return (x + 1) / 2; });

        // publish the new pdu
        message_port_pub(
            PMTCONSTSTR__pdu_out(),
            (pmt::cons(meta, pmt::init_u8vector(out_data.size(), out_data))));
    } else {
        GR_LOG_WARN(d_logger,
                    "Failed to 'from nrz' the data because it is not a f32 vector");
        message_port_pub(PMTCONSTSTR__pdu_out(), pdu);
    }
}


///////////////////////////////////////////////////////
// Slice [5,-4,6] to [1,0,1]
///////////////////////////////////////////////////////
void pdu_binary_tools_impl::handle_msg_slice(pmt::pmt_t pdu)
{
    // make sure PDU data is formed properly
    if (!(pmt::is_pair(pdu))) {
        GR_LOG_NOTICE(d_logger, "received unexpected PMT (non-pair)");
        return;
    }

    pmt::pmt_t meta = pmt::car(pdu);
    pmt::pmt_t v_data = pmt::cdr(pdu);

    // extract data
    if (pmt::is_f32vector(v_data)) {
        std::vector<float> in_data = pmt::f32vector_elements(v_data);
        std::vector<uint8_t> out_data(in_data.size());

        // flip every bit (xor)
        std::transform(in_data.begin(),
                       in_data.end(),
                       out_data.begin(),
                       [](float x) -> uint8_t { return x > 0; });

        // publish the new pdu
        message_port_pub(
            PMTCONSTSTR__pdu_out(),
            (pmt::cons(meta, pmt::init_u8vector(out_data.size(), out_data))));
    } else {
        GR_LOG_WARN(d_logger, "Failed to 'slice' the data because it is not a f32vector");
        message_port_pub(PMTCONSTSTR__pdu_out(), pdu);
    }
}

///////////////////////////////////////////////////////
// Endian swap is [1,0,1,0, 0,0,1,1] to [1,1,0,0 0,1,0,1]
///////////////////////////////////////////////////////
void pdu_binary_tools_impl::handle_msg_endian8(pmt::pmt_t pdu)
{
    // make sure PDU data is formed properly
    if (!(pmt::is_pair(pdu))) {
        GR_LOG_NOTICE(d_logger, "received unexpected PMT (non-pair)");
        return;
    }

    pmt::pmt_t meta = pmt::car(pdu);
    pmt::pmt_t v_data = pmt::cdr(pdu);

    // extract uint8 data
    if (pmt::is_u8vector(v_data)) {
        std::vector<uint8_t> in_data = pmt::u8vector_elements(v_data);
        std::vector<uint8_t> out_data(in_data.size());

        size_t div8 = in_data.size() / 8;
        // read 8 bits, write them in reverse order
        for (size_t i = 0; i < div8; i++) {
            for (size_t j = 0; j < 8; j++) {
                out_data[(i * 8) + j] = in_data[(i * 8 + 7) - j];
            }
        }

        // publish the new pdu
        message_port_pub(
            PMTCONSTSTR__pdu_out(),
            (pmt::cons(meta, pmt::init_u8vector(out_data.size(), out_data))));
    } else {
        GR_LOG_WARN(d_logger,
                    "Failed to endian-swap the data because it is not a u8vector");
        message_port_pub(PMTCONSTSTR__pdu_out(), pdu);
    }
}


///////////////////////////////////////////////////////
// Manchester Decode
// [0,1,1,0,0,1,1,0] -> [0,1,0,1]
// [-1,1,1,-1,-1,1,-1] -> [0,1,0,1]
///////////////////////////////////////////////////////
void pdu_binary_tools_impl::handle_msg_manchester_decode(pmt::pmt_t pdu)
{
    // make sure PDU data is formed properly
    if (!(pmt::is_pair(pdu))) {
        GR_LOG_NOTICE(d_logger, "received unexpected PMT (non-pair)");
        return;
    }

    pmt::pmt_t meta = pmt::car(pdu);
    pmt::pmt_t v_data = pmt::cdr(pdu);

    // extract uint8 data
    if (pmt::is_u8vector(v_data)) {
        std::vector<uint8_t> in_data = pmt::u8vector_elements(v_data);
        std::vector<uint8_t> out_data(in_data.size() >> 1, 0);

        for (size_t i = 0; i < out_data.size(); ++i) {
            if (in_data[2 * i + 1] < in_data[2 * i]) {
                // 1->0 transition
                out_data[i] = 1;
            }
        }

        // publish the new pdu
        message_port_pub(
            PMTCONSTSTR__pdu_out(),
            (pmt::cons(meta, pmt::init_u8vector(out_data.size(), out_data))));
    } else if (pmt::is_f32vector(v_data)) {
        std::vector<float> in_data = pmt::f32vector_elements(v_data);
        std::vector<float> out_data(in_data.size() >> 1, -1);

        for (size_t i = 0; i < out_data.size(); ++i) {
            if (in_data[2 * i + 1] < in_data[2 * i]) {
                // 1->0 transition
                out_data[i] = 1;
            }
        }

        // publish the new pdu
        message_port_pub(
            PMTCONSTSTR__pdu_out(),
            (pmt::cons(meta, pmt::init_f32vector(out_data.size(), out_data))));

    } else {
        GR_LOG_WARN(
            d_logger,
            "Failed to manchester decode the data because it is not a u8vector or float");
        message_port_pub(PMTCONSTSTR__pdu_out(), pdu);
    }
}

///////////////////////////////////////////////////////
// Manchester Encode
// [0,1,0,1] -> [0,1,1,0,0,1,1,0]
// [-1,1,-1,1] -> [-1,1,1,-1,-1,1,1,-1]
///////////////////////////////////////////////////////
void pdu_binary_tools_impl::handle_msg_manchester_encode(pmt::pmt_t pdu)
{
    // make sure PDU data is formed properly
    if (!(pmt::is_pair(pdu))) {
        GR_LOG_NOTICE(d_logger, "received unexpected PMT (non-pair)");
        return;
    }

    pmt::pmt_t meta = pmt::car(pdu);
    pmt::pmt_t v_data = pmt::cdr(pdu);

    // extract uint8 data
    if (pmt::is_u8vector(v_data)) {
        std::vector<uint8_t> in_data = pmt::u8vector_elements(v_data);
        std::vector<uint8_t> out_data(2 * in_data.size(), 0);

        for (size_t i = 0; i < in_data.size(); ++i) {
            if (in_data[i]) {
                out_data[2 * i] = 1;
                out_data[2 * i + 1] = 0;
            } else {
                out_data[2 * i] = 0;
                out_data[2 * i + 1] = 1;
            }
        }

        // publish the new pdu
        message_port_pub(
            PMTCONSTSTR__pdu_out(),
            (pmt::cons(meta, pmt::init_u8vector(out_data.size(), out_data))));
    } else if (pmt::is_f32vector(v_data)) {
        std::vector<float> in_data = pmt::f32vector_elements(v_data);
        std::vector<float> out_data(2 * in_data.size(), 0);

        for (size_t i = 0; i < in_data.size(); ++i) {
            if (in_data[i] > 0) {
                out_data[2 * i] = 1;
                out_data[2 * i + 1] = -1;
            } else {
                out_data[2 * i] = -1;
                out_data[2 * i + 1] = 1;
            }
        }

        // publish the new pdu
        message_port_pub(
            PMTCONSTSTR__pdu_out(),
            (pmt::cons(meta, pmt::init_f32vector(out_data.size(), out_data))));

    } else {
        GR_LOG_WARN(d_logger,
                    "Failed to endian-swap the data because it is not a u8vector");
        message_port_pub(PMTCONSTSTR__pdu_out(), pdu);
    }
}


} /* namespace pdu_utils */
} /* namespace gr */
