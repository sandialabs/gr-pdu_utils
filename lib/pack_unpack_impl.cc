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

#include "pack_unpack_impl.h"
#include <gnuradio/io_signature.h>
#include <boost/format.hpp>
namespace gr {
namespace pdu_utils {

pack_unpack::sptr pack_unpack::make(uint32_t mode, uint32_t bit_order)
{
    return gnuradio::make_block_sptr<pack_unpack_impl>(mode, bit_order);
}

/*
 * The private constructor
 */
pack_unpack_impl::pack_unpack_impl(uint32_t mode, uint32_t bit_order)
    : gr::block("pack_unpack", io_signature::make(0, 0, 0), io_signature::make(0, 0, 0)),
      d_mode(mode),
      d_bit_order(bit_order)
{
    message_port_register_in(PMTCONSTSTR__pdu_in());
    set_msg_handler(PMTCONSTSTR__pdu_in(),
                    [this](pmt::pmt_t msg) { this->handle_msg(msg); });
    message_port_register_out(PMTCONSTSTR__pdu_out());

    if (d_mode == MODE_UNPACK_BYTE) {
        d_logger->info("PDU Pack/Unpack instantiated in UNPACK BYTE mode");
    } else if (d_mode == MODE_PACK_BYTE) {
        d_logger->info("PDU Pack/Unpack instantiated in PACK BYTE mode");
    } else if (d_mode == MODE_BITSWAP_BYTE) {
        d_logger->info("PDU Pack/Unpack instantiated in BITSWAP mode");
    } else {
        d_logger->warn("PDU Pack/Unpack instantiated in UNKNOWN mode");
    }
}

/*
 * Our virtual destructor.
 */
pack_unpack_impl::~pack_unpack_impl() {}


void pack_unpack_impl::handle_msg(pmt::pmt_t pdu)
{
    // make sure PDU data is formed properly
    if (!(pmt::is_pair(pdu))) {
        d_logger->notice("received unexpected PMT (non-pair)");
        return;
    }

    gr::thread::scoped_lock l(d_setlock);

    pmt::pmt_t meta = pmt::car(pdu);
    pmt::pmt_t v_data = pmt::cdr(pdu);

    if (!(is_dict(meta) && pmt::is_uniform_vector(v_data))) {
        d_logger->notice("received unexpected PMT (non-PDU)");
        return;
    }

    uint32_t v_len = pmt::length(v_data);
   // size_t v_itemsize = pmt::uniform_vector_itemsize(v_data);

    if (d_mode == MODE_UNPACK_BYTE) {
        /*
         * UNPACK BYTE: input is an aray of bytes to be converted to bits. The
         * input can be any byte vector.
         */
        if (!pmt::is_u8vector(v_data)) {
            d_logger->error("PDU vector is not uint8, dropping");
            return;
        }
        const std::vector<uint8_t> data = pmt::u8vector_elements(v_data);
        std::vector<uint8_t> out;

        for (size_t ii = 0; ii < data.size(); ii++) {
            if (d_bit_order == BIT_ORDER_LSB_FIRST) {
                // START LSB FIRST PROCESSING
                for (size_t jj = 0; jj < 8; jj++) {
                    out.push_back((data[ii] >> jj) & 0x01);
                }
                // END LSB FIRST PROCESSING
            } else {
                // START MSB FIRST PROCESSING
                for (int jj = 7; jj >= 0; jj--) {
                    out.push_back((data[ii] >> jj) & 0x01);
                }
                // END MSB FIRST PROCESSING
            }
        }

        message_port_pub(PMTCONSTSTR__pdu_out(),
                         pmt::cons(meta, pmt::init_u8vector(out.size(), out)));


    } else if (d_mode == MODE_PACK_BYTE) {
        /*
         * PACK BYTE: input is an aray of bits to be converted to bytes. The
         * input should be a multiple of 8 bytes and  will be zero padded to
         * multiple of 8 bytes (same behavior as numpy.packbits() has).
         * Values other than zero will map to '1'.
         */
        if (!pmt::is_u8vector(v_data)) {
            d_logger->error("PDU vector is not uint8, dropping");
            return;
        }

        const std::vector<uint8_t> data_in = pmt::u8vector_elements(v_data);
        std::vector<uint8_t> data = data_in;
        std::vector<uint8_t> out;

        uint32_t nbytes = data.size() / 8;
        if (data.size() % 8)
            nbytes++;
        data.resize(nbytes * 8, 0);
        out.reserve(nbytes);

        uint8_t byte = 0;
        for (size_t ii = 0; ii < data.size(); ii++) {
            if (d_bit_order == BIT_ORDER_LSB_FIRST) {
                // START LSB FIRST PROCESSING
                byte >>= 1;
                if (data[ii])
                    byte |= 0x80;
                if ((ii % 8) == 7) {
                    out.push_back(byte);
                    byte = 0;
                }
                // END LSB FIRST PROCESSING
            } else {
                // START MSB FIRST PROCESSING
                byte <<= 1;
                if (data[ii])
                    byte |= 0x01;
                if ((ii % 8) == 7) {
                    out.push_back(byte);
                    byte = 0;
                }
                // END MSB FIRST PROCESSING
            }
        }

        message_port_pub(PMTCONSTSTR__pdu_out(),
                         pmt::cons(meta, pmt::init_u8vector(out.size(), out)));


    } else if (d_mode == MODE_BITSWAP_BYTE) {
        /*
         * BITSWAP BYTE: input is a packed byte and output will have bit order
         * reversed on a byte-by-byte basis.
         */

        if (!pmt::is_u8vector(v_data)) {
            d_logger->error("PDU vector is not uint8, dropping");
            return;
        }

        std::vector<uint8_t> data = pmt::u8vector_elements(v_data);
        std::vector<uint8_t> out;
        out.reserve(v_len);
        for (size_t ii = 0; ii < v_len; ii++) {
            out.push_back(bitreverse[data[ii]]);
        }

        message_port_pub(PMTCONSTSTR__pdu_out(),
                         pmt::cons(meta, pmt::init_u8vector(out.size(), out)));


    } else {
        /*
         * All other modes are undefined... print a warning and drop PDU
         */
        d_logger->warn("Unknown block mode {}",d_mode);
    }
}

void pack_unpack_impl::set_mode(uint32_t mode)
{
    gr::thread::scoped_lock l(d_setlock);

    d_mode = mode;
}


void pack_unpack_impl::set_bit_order(uint32_t order)
{
    gr::thread::scoped_lock l(d_setlock);

    d_bit_order = order;
}

} /* namespace pdu_utils */
} /* namespace gr */
