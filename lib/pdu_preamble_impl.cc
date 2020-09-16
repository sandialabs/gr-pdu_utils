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

#include "pdu_preamble_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace pdu_utils {

/**
 * Creates a pdu_preamble and returns a shared pointer
 *
 * @param preable - array to be inserted at the begining of the PDU data
 * @param tail - array to be inserted after the PDU data
 * @param interp - interpolation factor of each value
 * @param zero_pad - number of zeros inserted at the start and end of the PDU
 * @param nrz - Non Return to Zero encoding mode
 * @return pdu_preamble::sptr - shared pointer to pdu_preamble instance
 */
pdu_preamble::sptr pdu_preamble::make(const std::vector<uint8_t> preamble,
                                      const std::vector<uint8_t> tail,
                                      uint32_t interp,
                                      uint32_t zero_pad,
                                      bool nrz)
{
    return gnuradio::get_initial_sptr(
        new pdu_preamble_impl(preamble, tail, interp, zero_pad, nrz));
}

/**
 * Constructor
 *
 * @param preamble - array to be inserted at the begining of the PDU data
 * @param tail - array to be inserted after the PDU data
 * @param interp - interpolation factor of each value, default = 1
 * @param zero_pad - number of zeros inserted at the start and end of the PDU, default = 0
 * @param nrz - Non Return to Zero encoding mode, default = true
 */
pdu_preamble_impl::pdu_preamble_impl(const std::vector<uint8_t> preamble,
                                     const std::vector<uint8_t> tail,
                                     uint32_t interp,
                                     uint32_t zero_pad,
                                     bool nrz)
    : gr::block("pdu_preamble", io_signature::make(0, 0, 0), io_signature::make(0, 0, 0)),
      d_preamble(preamble),
      d_tail(tail),
      d_zero_pad(zero_pad),
      d_nrz(nrz)
{
    set_interp(interp);
    set_zero_pad(d_zero_pad);

    message_port_register_in(PMTCONSTSTR__PDU_IN);
    set_msg_handler(PMTCONSTSTR__PDU_IN,
                    boost::bind(&pdu_preamble_impl::handle_msg, this, _1));
    message_port_register_out(PMTCONSTSTR__PDU_OUT);
} // end constructor

/*
 * Our virtual destructor.
 */
pdu_preamble_impl::~pdu_preamble_impl() {}

void pdu_preamble_impl::handle_msg(pmt::pmt_t pdu)
{
    // make sure PDU data is formed properly
    if (!(pmt::is_pair(pdu))) {
        GR_LOG_NOTICE(d_logger, "received unexpected PMT (non-pair)");
        return;
    }

    gr::thread::scoped_lock l(d_setlock);

    pmt::pmt_t meta = pmt::car(pdu);
    pmt::pmt_t v_data = pmt::cdr(pdu);

    // make sure PDU data is formed properly
    if (!(is_dict(meta) && pmt::is_uniform_vector(v_data))) {
        GR_LOG_NOTICE(d_logger, "received unexpected PMT (non-PDU)");
        return;
    }

    uint32_t v_len = pmt::length(v_data);
    size_t v_itemsize = pmt::uniform_vector_itemsize(v_data);

    // make sure data is u8 vector
    if (!(pmt::is_u8vector(v_data))) {
        GR_LOG_NOTICE(d_logger, "PDU Preamble: PDU data is not uint8!");
        return;
    }

    GR_LOG_NOTICE(d_logger, boost::format("NRZ Mode %d") % d_nrz);


    const std::vector<uint8_t> data_in = pmt::u8vector_elements(v_data);
    std::vector<float> out; // output
    out.clear();
    out.reserve(d_zeros.size() * 2 + d_preamble_interp.size() + d_tail_interp.size() +
                (v_len * d_interp));
    out.insert(out.end(), d_zeros.begin(), d_zeros.end());
    out.insert(out.end(), d_preamble_interp.begin(), d_preamble_interp.end());

    for (int ii = 0; ii < data_in.size(); ii++) {
        if (d_nrz) {
            float x = (data_in[ii] > 0) ? 1.0 : -1.0;
            for (int jj = 0; jj < d_interp; jj++) {
                out.push_back(x);
            }
        } else {
            for (int jj = 0; jj < d_interp; jj++) {
                out.push_back(data_in[ii]);
            }
        }
    }

    out.insert(out.end(), d_tail_interp.begin(), d_tail_interp.end());
    out.insert(out.end(), d_zeros.begin(), d_zeros.end());

    // std::cout << "PREAMBLE!: ["; for (int ii = 0; ii < out.size()-1; ii++)  std::cout
    // << out[ii] << ", "; std::cout << out.back() << "]" << std::endl;

    message_port_pub(PMTCONSTSTR__PDU_OUT,
                     pmt::cons(meta, pmt::init_f32vector(out.size(), out)));
}


/**
 * Set the preamble array
 *
 * @param preamble - preamble array thats inserted before PDU
 */
void pdu_preamble_impl::set_preamble(const std::vector<uint8_t> preamble)
{
    gr::thread::scoped_lock l(d_setlock);

    d_preamble = preamble;
    d_preamble_interp.clear();
    if (d_preamble.size()) {
        for (int ii = 0; ii < d_preamble.size(); ii++) {
            if (d_nrz) {
                float x = (d_preamble[ii] > 0) ? 1.0 : -1.0;
                for (int jj = 0; jj < d_interp; jj++) {
                    d_preamble_interp.push_back(x);
                }
            } else {
                for (int jj = 0; jj < d_interp; jj++) {
                    d_preamble_interp.push_back(d_preamble[ii]);
                }
            }
        }
    }
}


/**
 * Set the tail array
 *
 * @param tail - tail array thats inserted after the PDU
 */
void pdu_preamble_impl::set_tail(const std::vector<uint8_t> tail)
{
    gr::thread::scoped_lock l(d_setlock);

    d_tail = tail;
    d_tail_interp.clear();
    if (d_tail.size()) {
        for (int ii = 0; ii < d_tail.size(); ii++) {
            if (d_nrz) {
                float x = (d_tail[ii] > 0) ? 1.0 : -1.0;
                for (int jj = 0; jj < d_interp; jj++) {
                    d_tail_interp.push_back(x);
                }
            } else {
                for (int jj = 0; jj < d_interp; jj++) {
                    d_tail_interp.push_back(d_tail[ii]);
                }
            }
        }
    }
}


/**
 * Sets the number of zeros pading the front and rear of the encoded PDU
 *
 * @param zero_pad - number of zeros to pad with
 */
void pdu_preamble_impl::set_zero_pad(uint32_t zero_pad)
{
    gr::thread::scoped_lock l(d_setlock);

    d_zero_pad = zero_pad;
    d_zeros.clear();
    d_zeros.resize(d_zero_pad);
    std::fill(d_zeros.begin(), d_zeros.end(), 0);
}


/**
 * Sets the interpolation factor
 *
 * @param interp - desired interpolation value
 */
void pdu_preamble_impl::set_interp(uint32_t interp)
{
    d_interp = interp;
    set_preamble(d_preamble);
    set_tail(d_tail);
}


/**
 * Sets the Non Return to Zero( NRZ ) behavior
 *
 * @parma nrz - desired nrz value
 */
void pdu_preamble_impl::set_nrz(bool nrz)
{
    d_nrz = nrz;
    set_preamble(d_preamble);
    set_tail(d_tail);
}

} /* namespace pdu_utils */
} /* namespace gr */
