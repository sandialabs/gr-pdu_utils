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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "pdu_binary_tools_impl.h"
#include "pdu_utils/constants.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace pdu_utils {

pdu_binary_tools::sptr pdu_binary_tools::make(uint8_t mode)
{
    return gnuradio::get_initial_sptr(new pdu_binary_tools_impl(mode));
}

/*
 * The private constructor
 */
pdu_binary_tools_impl::pdu_binary_tools_impl(uint8_t mode)
    : gr::sync_block("pdu_binary_tools",
                     gr::io_signature::make(0, 0, 0),
                     gr::io_signature::make(0, 0, 0))
{

    // This block will handle a lot of really simply binary transforms
    // based on some enum MODE

    // TO DO: bit flip, 0/1 to -1/1, -1/1 to 0/1, slice, ...?

    // register message ports
    message_port_register_in(PMTCONSTSTR__PDU_IN);
    message_port_register_out(PMTCONSTSTR__PDU_OUT);


    // bit flip mode
    switch (mode) {
    case pdu_binary_tools::BIT_FLIP: {
        std::cout << "pdu binary tools in BIT FLIP mode" << std::endl;
        set_msg_handler(
            PMTCONSTSTR__PDU_IN,
            boost::bind(&pdu_binary_tools_impl::handle_msg_bit_flip, this, _1));
        break;
    }
    case pdu_binary_tools::TO_NRZ: {
        std::cout << "pdu binary tools in TO NRZ mode" << std::endl;
        set_msg_handler(PMTCONSTSTR__PDU_IN,
                        boost::bind(&pdu_binary_tools_impl::handle_msg_to_nrz, this, _1));
        break;
    }
    case pdu_binary_tools::FROM_NRZ: {
        std::cout << "pdu binary tools in FROM NRZ mode" << std::endl;
        set_msg_handler(
            PMTCONSTSTR__PDU_IN,
            boost::bind(&pdu_binary_tools_impl::handle_msg_from_nrz, this, _1));
        break;
    }
    case pdu_binary_tools::SLICE: {
        std::cout << "pdu binary tools in SLICE mode" << std::endl;
        set_msg_handler(PMTCONSTSTR__PDU_IN,
                        boost::bind(&pdu_binary_tools_impl::handle_msg_slice, this, _1));
        break;
    }
    case pdu_binary_tools::ENDIAN_SWAP8: {
        std::cout << "pdu binary tools in ENDIAN_SWAP8 mode" << std::endl;
        set_msg_handler(
            PMTCONSTSTR__PDU_IN,
            boost::bind(&pdu_binary_tools_impl::handle_msg_endian8, this, _1));
        break;
    }

    default: {
        set_msg_handler(
            PMTCONSTSTR__PDU_IN,
            boost::bind(&pdu_binary_tools_impl::handle_msg_passthrough, this, _1));
    }
    }
}

/*
 * Our virtual destructor.
 */
pdu_binary_tools_impl::~pdu_binary_tools_impl() {}

void pdu_binary_tools_impl::handle_msg_passthrough(pmt::pmt_t pdu)
{
    message_port_pub(PMTCONSTSTR__PDU_OUT, pdu);
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
        message_port_pub(PMTCONSTSTR__PDU_OUT,
                         (pmt::cons(meta, pmt::init_u8vector(data.size(), data))));
    } else {
        std::cout << "Failed to bit flip the data because it is not a u8vector"
                  << std::endl;
        message_port_pub(PMTCONSTSTR__PDU_OUT, pdu);
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
            PMTCONSTSTR__PDU_OUT,
            (pmt::cons(meta, pmt::init_f32vector(out_data.size(), out_data))));
    } else {
        std::cout << "Failed to 'to nrz' the data because it is not a u8vector"
                  << std::endl;
        message_port_pub(PMTCONSTSTR__PDU_OUT, pdu);
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
            PMTCONSTSTR__PDU_OUT,
            (pmt::cons(meta, pmt::init_u8vector(out_data.size(), out_data))));
    } else {
        std::cout << "Failed to 'from nrz' the data because it is not a f32 vector"
                  << std::endl;
        message_port_pub(PMTCONSTSTR__PDU_OUT, pdu);
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
            PMTCONSTSTR__PDU_OUT,
            (pmt::cons(meta, pmt::init_u8vector(out_data.size(), out_data))));
    } else {
        std::cout << "Failed to 'slice' the data because it is not a f32vector"
                  << std::endl;
        message_port_pub(PMTCONSTSTR__PDU_OUT, pdu);
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
            PMTCONSTSTR__PDU_OUT,
            (pmt::cons(meta, pmt::init_u8vector(out_data.size(), out_data))));
    } else {
        std::cout << "Failed to endian-swap the data because it is not a u8vector"
                  << std::endl;
        message_port_pub(PMTCONSTSTR__PDU_OUT, pdu);
    }
}


int pdu_binary_tools_impl::work(int noutput_items,
                                gr_vector_const_void_star& input_items,
                                gr_vector_void_star& output_items)
{
    return noutput_items;
}

} /* namespace pdu_utils */
} /* namespace gr */
