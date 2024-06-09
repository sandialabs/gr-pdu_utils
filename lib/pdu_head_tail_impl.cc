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

#include "pdu_head_tail_impl.h"
#include <gnuradio/io_signature.h>
#include <boost/format.hpp>
namespace gr {
namespace pdu_utils {

pdu_head_tail::sptr
pdu_head_tail::make(uint32_t input_type, uint32_t length, uint32_t histsize)
{
    return gnuradio::make_block_sptr<pdu_head_tail_impl>(input_type, length, histsize);
}

/*
 * The private constructor
 */
pdu_head_tail_impl::pdu_head_tail_impl(uint32_t input_type,
                                       uint32_t length,
                                       uint32_t histsize)
    : gr::block(
          "pdu_head_tail", io_signature::make(0, 0, 0), io_signature::make(0, 0, 0)),
      d_input_type(input_type),
      d_length(length),
      d_maxhistsize(histsize),
      d_histsize(0),
      d_space_bytes(false),
      d_bit_order(BIT_ORDER_LSB_FIRST)
{
    if (d_input_type == INPUTTYPE_UNPACKED_BYTE) {
        d_logger->debug("PDU HEAD/TAIL block operating in Unpacked U8 PDU mode");
    } else if (d_input_type == INPUTTYPE_PACKED_BYTE) {
        // d_logger->debug("PDU HEAD/TAIL block operating in 'Packed U8 PDU'
        // mode");
        d_logger->fatal("PACKED BYTE MODE NOT SUPPORTED YET");
        throw std::invalid_argument("invalid mode PACKED BYTE");
    } else if (d_input_type == INPUTTYPE_FLOAT) {
        d_logger->debug("PDU HEAD/TAIL block operating in FLOAT PDU mode");
    } else {
        std::ostringstream msg;
        msg << boost::format("PDU HEAD/TAIL block instantiated in unknown mode %d") %
                d_input_type;
        d_logger->fatal(msg.str());
        throw std::invalid_argument("unknown mode");
    }

    message_port_register_in(PMTCONSTSTR__pdu_in());
    set_msg_handler(PMTCONSTSTR__pdu_in(),
                    [this](pmt::pmt_t msg) { this->handle_pdu(msg); });
    message_port_register_out(PMTCONSTSTR__head());
    message_port_register_out(PMTCONSTSTR__tail());

    reset();
}

/*
 * Our virtual destructor.
 */
pdu_head_tail_impl::~pdu_head_tail_impl() {}

void pdu_head_tail_impl::handle_pdu(pmt::pmt_t pdu)
{
    // make sure PDU data is formed properly
    if (!(pmt::is_pair(pdu))) {
        GR_LOG_WARN(d_logger, "received unexpected PMT (non-pair)");
        return;
    }

    gr::thread::scoped_lock l(d_setlock);

    /* code */
    pmt::pmt_t meta = pmt::car(pdu);
    pmt::pmt_t v_data = pmt::cdr(pdu);

    std::vector<uint8_t> head_bits, tail_bits;
    head_bits.resize(d_length, 0);
    tail_bits.resize(d_length, 0);
    std::vector<float> head_bits_f, tail_bits_f;
    head_bits_f.resize(d_length, 0);
    tail_bits_f.resize(d_length, 0);

    if (d_input_type == INPUTTYPE_UNPACKED_BYTE) {
        if (!(pmt::is_dict(meta) && pmt::is_u8vector(v_data))) {
            GR_LOG_WARN(d_logger, "PMT is not a U8 PDU, dropping");
            return;
        }

        const std::vector<uint8_t> data = pmt::u8vector_elements(v_data);
        uint32_t loop_size = data.size();
        if (loop_size > d_length)
            loop_size = d_length;

        // convert head to binary
        for (size_t ii = 0; ii < loop_size; ii++) {
            head_bits[ii] = (data[ii] == 0) ? 0 : 1;
        }

        int tail_idx = data.size() - d_length;
        if (tail_idx < 0)
            tail_idx = 0;
        for (size_t ii = (d_length - loop_size); ii < d_length; ii++) {
            tail_bits[ii] = (data[tail_idx++] == 0) ? 0 : 1;
        }

    } else if (d_input_type == INPUTTYPE_FLOAT) {
        if (!(pmt::is_dict(meta) && pmt::is_f32vector(v_data))) {
            GR_LOG_WARN(d_logger, "PMT is not a F32 PDU, dropping");
            return;
        }

        const std::vector<float> data = pmt::f32vector_elements(v_data);
        uint32_t loop_size = data.size();
        if (loop_size > d_length)
            loop_size = d_length;

        // convert head to binary
        for (size_t ii = 0; ii < loop_size; ii++) {
            head_bits_f[ii] = (data[ii]); // == 0) ? 0 : 1;
        }

        int tail_idx = data.size() - d_length;
        if (tail_idx < 0)
            tail_idx = 0;
        for (size_t ii = (d_length - loop_size); ii < d_length; ii++) {
            tail_bits_f[ii] = (data[tail_idx++]); // == 0) ? 0 : 1;
        }

    } else if (d_input_type == INPUTTYPE_PACKED_BYTE) {
        if (!(pmt::is_dict(meta) && pmt::is_u8vector(v_data))) {
            GR_LOG_WARN(d_logger, "PMT is not a U8 PDU, dropping");
            return;
        }

        const std::vector<uint8_t> data = pmt::u8vector_elements(v_data);
        uint32_t loop_size = data.size();
        if (loop_size > d_length)
            loop_size = d_length;

        // unpack the input PDU
        uint8_t head_byte = 0;
        //uint8_t tail_byte = 0;
        for (size_t ii = 0; ii < d_length; ii++) {
            if (d_bit_order == BIT_ORDER_LSB_FIRST) {
                // START LSB FIRST PROCESSING
                head_byte >>= 1;
                if (data[ii])
                    head_byte |= 0x80;
                if ((ii % 8) == 7) {
                    head_bits[ii] = head_byte;
                    head_byte = 0;
                }
                // END LSB FIRST PROCESSING
            } else {
                // START MSB FIRST PROCESSING
                head_byte <<= 1;
                if (data[ii])
                    head_byte |= 0x01;
                if ((ii % 8) == 7) {
                    head_bits[ii] = head_byte;
                    head_byte = 0;
                }
                // END MSB FIRST PROCESSING
            }
        }
    }

    /*
    std::cout << "head: [" << int(head_bits[0]);
    for (size_t ii=1;ii<d_length;ii++) std::cout << ", " << int(head_bits[ii]);
    std::cout << "]" << std::endl;

    std::cout << "tail: [" << int(tail_bits[0]);
    for (size_t ii=1;ii<d_length;ii++) std::cout << ", " << int(tail_bits[ii]);
    std::cout << "]" << std::endl;
    */

    if (d_input_type == INPUTTYPE_FLOAT) {
        if (d_histsize < d_maxhistsize) {
            uint32_t s = d_histsize * d_length;
            // append the bit vectors
            for (size_t ii = 0; ii < d_length; ii++) {
                d_head_f[s + ii] = head_bits_f[ii];
                d_tail_f[s + ii] = tail_bits_f[ii];
            }
            d_histsize++;
        } else {
            uint32_t s = d_maxhistsize * d_length;
            // shift data forward
            for (size_t ii = 0; ii < (s - d_length); ii++) {
                d_head_f[ii] = d_head_f[ii + d_length];
                d_tail_f[ii] = d_tail_f[ii + d_length];
            }
            // append the bit vector
            s -= d_length;
            for (size_t ii = 0; ii < d_length; ii++) {
                d_head_f[s + ii] = head_bits_f[ii];
                d_tail_f[s + ii] = tail_bits_f[ii];
            }
        }

        message_port_pub(
            PMTCONSTSTR__head(),
            (pmt::cons(meta, pmt::init_f32vector(d_histsize * d_length, d_head_f))));
        message_port_pub(
            PMTCONSTSTR__tail(),
            (pmt::cons(meta, pmt::init_f32vector(d_histsize * d_length, d_tail_f))));

    } else {
        if (d_histsize < d_maxhistsize) {
            uint32_t s = d_histsize * d_length;
            // append the bit vectors
            for (size_t ii = 0; ii < d_length; ii++) {
                d_head[s + ii] = head_bits[ii];
                d_tail[s + ii] = tail_bits[ii];
            }
            d_histsize++;
        } else {
            uint32_t s = d_maxhistsize * d_length;
            // shift data forward
            for (size_t ii = 0; ii < (s - d_length); ii++) {
                d_head[ii] = d_head[ii + d_length];
                d_tail[ii] = d_tail[ii + d_length];
            }
            // append the bit vector
            s -= d_length;
            for (size_t ii = 0; ii < d_length; ii++) {
                d_head[s + ii] = head_bits[ii];
                d_tail[s + ii] = tail_bits[ii];
            }
        }

        /*
        std::cout << "D_head: [" << int(d_head[0]);
        for (size_t ii=1;ii<d_histsize*d_length;ii++) std::cout << ", " << int(d_head[ii]);
        std::cout << "]" << std::endl;

        std::cout << "D_tail: [" << int(d_tail[0]);
        for (size_t ii=1;ii<d_histsize*d_length;ii++) std::cout << ", " << int(d_tail[ii]);
        std::cout << "]" << std::endl;
        */

        message_port_pub(
            PMTCONSTSTR__head(),
            (pmt::cons(meta, pmt::init_u8vector(d_histsize * d_length, d_head))));
        message_port_pub(
            PMTCONSTSTR__tail(),
            (pmt::cons(meta, pmt::init_u8vector(d_histsize * d_length, d_tail))));
    }
}


void pdu_head_tail_impl::set_length(uint32_t length)
{
    gr::thread::scoped_lock l(d_setlock);

    d_length = length;
    reset();
}


void pdu_head_tail_impl::set_histsize(uint32_t histsize)
{
    gr::thread::scoped_lock l(d_setlock);

    d_maxhistsize = histsize;
    reset();
}


void pdu_head_tail_impl::set_space_bytes(bool spacebytes)
{
    gr::thread::scoped_lock l(d_setlock);

    d_space_bytes = spacebytes;
    reset();
}


void pdu_head_tail_impl::reset(void)
{
    d_histsize = 0;
    d_head.clear();
    d_tail.clear();
    d_head.resize(d_length * d_maxhistsize, 0);
    d_tail.resize(d_length * d_maxhistsize, 0);
    d_head_f.clear();
    d_tail_f.clear();
    d_head_f.resize(d_length * d_maxhistsize, 0);
    d_tail_f.resize(d_length * d_maxhistsize, 0);
}

} /* namespace pdu_utils */
} /* namespace gr */
