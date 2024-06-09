/* -*- c++ -*- */
/*
 * Copyright 2018, 2019, 2020 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "pdu_set_m_impl.h"
#include <gnuradio/pdu_utils/pdu_set_m.h>
#include <gnuradio/io_signature.h>
#include <algorithm>
#include <boost/format.hpp>
namespace gr {
namespace pdu_utils {


pdu_set_m::sptr pdu_set_m::make(pmt::pmt_t k, pmt::pmt_t v, bool kv_merge, bool v_overwrite)
{
    return gnuradio::make_block_sptr<pdu_utils::pdu_set_m_impl>(k, v, kv_merge, v_overwrite);
}

/*
 * The private constructor
 */
pdu_set_m_impl::pdu_set_m_impl(pmt::pmt_t k, pmt::pmt_t v, bool kv_merge, bool v_overwrite)
    : gr::block("pdu_set_m", io_signature::make(0, 0, 0), io_signature::make(0, 0, 0)),
      d_k(k),
      d_v(v),
      d_kv_merge(kv_merge),
      d_v_overwrite(v_overwrite)
{
    message_port_register_in(PMTCONSTSTR__pdu_in());
    set_msg_handler(PMTCONSTSTR__pdu_in(),
                    [this](pmt::pmt_t msg) { this->handle_msg(msg); });
    message_port_register_in(PMTCONSTSTR__ctrl());
    set_msg_handler(PMTCONSTSTR__ctrl(),
                    [this](pmt::pmt_t msg) { this->handle_ctrl_msg(msg); });
    message_port_register_out(PMTCONSTSTR__pdu_out());
}

/*
 * Our virtual destructor.
 */
pdu_set_m_impl::~pdu_set_m_impl() {}


void pdu_set_m_impl::setup_rpc()
{
#ifdef GR_CTRLPORT

    add_rpc_variable(rpcbasic_sptr(new rpcbasic_register_handler<pdu_set_m>(
        alias(), "ctrl", "", "ctrl", RPC_PRIVLVL_MIN, DISPNULL)));

#endif /* GR_CTRLPORT */
}


/*
 * The following handler is responsible for accepting commands to set the
 * various controllable variables using their public methods.
 */
void pdu_set_m_impl::handle_ctrl_msg(pmt::pmt_t msg)
{
    gr::thread::scoped_lock l(d_setlock);

    if (pmt::is_pair(msg)) {
        if (pmt::eqv(pmt::car(msg), PMTCONSTSTR__val())) {
            std::ostringstream msgx;
            msgx << "value is %1%" << val();
            d_logger->notice(msgx.str());
        } else if (pmt::eqv(pmt::car(msg), PMTCONSTSTR__key())) {
            std::ostringstream msgx;
            msgx << "key is %1%" << key();
            d_logger->notice(msgx.str());
        } else if (pmt::eqv(pmt::car(msg), PMTCONSTSTR__set_val())) {
            set_val(pmt::cdr(msg));
            std::ostringstream msgx;
            msgx << "value set to %1%" << val();
            d_logger->notice(msgx.str());
        } else if (pmt::eqv(pmt::car(msg), PMTCONSTSTR__set_key())) {
            set_key(pmt::cdr(msg));
            std::ostringstream msgx;
            msgx << "key set to %1%" << key();
            d_logger->debug(msgx.str());
        } else {
            std::ostringstream msgx;
            msgx << "invalid command %1% received..." << pmt::car(msg);
            d_logger->warn(msgx.str());
        }
    } else {
        d_logger->warn( "received unexpected PMT command (non-pair)");
    }
}


void pdu_set_m_impl::handle_msg(pmt::pmt_t pdu)
{
    // make sure PDU data is formed properly
    if (!(pmt::is_pair(pdu))) {
        d_logger->warn("received unexpected PMT (non-pair)");
        return;
    }

    gr::thread::scoped_lock l(d_setlock);

    /* code */
    pmt::pmt_t meta = pmt::car(pdu);

    if (!pmt::is_dict(meta)) {
        d_logger->warn("received malformed PDU");
        return;
    }

    if (!d_v_overwrite && pmt::dict_has_key(meta, d_k)) {
        d_logger->warn("key already found, and overwrite disabled");
        return;
    }

    if (d_kv_merge) {
        meta = parse_val(meta);
    }
    else {
        meta = pmt::dict_add(meta, d_k, d_v);
    }
    message_port_pub(PMTCONSTSTR__pdu_out(), pmt::cons(meta, pmt::cdr(pdu)));
}

pmt::pmt_t pdu_set_m_impl::parse_val(pmt::pmt_t dict)
{
    std::string out_val;
    std::string in_val = pmt::symbol_to_string(d_v);
    std::string tmp;
    pmt::pmt_t refval;
    // if there are no values to parse, add key/val pair as seen and return
    if (!(std::count(in_val.begin(), in_val.end(), '{'))) {
        pmt::dict_add(dict, d_k, d_v);
        return dict;
    }
    while (in_val.size()) {
        // if there are no more left braces, append the rest of the val
        if (!std::count(in_val.begin(), in_val.end(), '{')) {
            out_val += in_val;
            break;
        }

        // if right brace appears before left brace, invalid syntax
        if (in_val.find('}') < in_val.find('{')) {
            d_logger->warn("unable to parse val string");
            return dict;
        }
        // read up to first brace and add to output string
        tmp = in_val.substr(0, in_val.find('{'));
        out_val += tmp;
        in_val.erase(0, in_val.find('{')+1);
        // if there's no right brace after a left brace, invalid syntax
        if (in_val.find('}') > in_val.size()) {
            d_logger->warn("unable to parse val string");
            return dict;
        }

        // read in key to parse the metadata for
        tmp = in_val.substr(0, in_val.find('}'));
        in_val.erase(0, in_val.find('}')+1);
        // if found, return value; otherwise, return key
        refval = pmt::dict_ref(dict, pmt::intern(tmp), pmt::intern(tmp));
        // is_bool, is_integer, is_uint64, is_real, is_complex, is_symbol
        if (pmt::is_bool(refval)) {
            out_val += (pmt::is_true(refval)) ? "true" : "false";
        } else if (pmt::is_integer(refval)) {
            out_val += std::to_string(pmt::to_long(refval));
        } else if (pmt::is_uint64(refval)) {
            out_val += std::to_string(pmt::to_uint64(refval));
        } else if (pmt::is_real(refval)) {
            out_val += std::to_string(pmt::to_double(refval));
        } else if (pmt::is_complex(refval)) {
            std::complex<double> compval = pmt::to_complex(refval);
            out_val += std::to_string(compval.real()) + "+" + std::to_string(compval.imag()) + "i";
        } else if (pmt::is_symbol(refval)) {
            out_val += pmt::symbol_to_string(refval);
        } else {
            d_logger->warn("value type of {} not supported",tmp);
            out_val += tmp;
        }
    }
    dict = pmt::dict_add(dict, d_k, pmt::intern(out_val));
    return dict;
}

void pdu_set_m_impl::set_key(pmt::pmt_t k)
{
    gr::thread::scoped_lock l(d_setlock);

    d_k = k;
}


void pdu_set_m_impl::set_val(pmt::pmt_t v)
{
    gr::thread::scoped_lock l(d_setlock);

    d_v = v;
}

void pdu_set_m_impl::set_kv(pmt::pmt_t k, pmt::pmt_t v)
{
    gr::thread::scoped_lock l(d_setlock);

    d_k = k;
    d_v = v;
}

void pdu_set_m_impl::set_kv_merge(bool kv_merge)
{
    gr::thread::scoped_lock l(d_setlock);

    d_kv_merge = kv_merge;
}

void pdu_set_m_impl::set_v_overwrite(bool v_overwrite)
{
    gr::thread::scoped_lock l(d_setlock);

    d_v_overwrite = v_overwrite;
}

} /* namespace pdu_utils */
} /* namespace gr */
