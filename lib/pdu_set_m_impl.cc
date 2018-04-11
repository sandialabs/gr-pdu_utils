/* -*- c++ -*- */
/*
 * Copyright 2018 National Technology & Engineering Solutions of Sandia, LLC (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government retains certain rights in this software.
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

#include <gnuradio/io_signature.h>
#include "pdu_set_m_impl.h"

namespace gr {
  namespace pdu_utils {

    pdu_set_m::sptr
    pdu_set_m::make(pmt::pmt_t k, pmt::pmt_t v)
    {
      return gnuradio::get_initial_sptr
        (new pdu_set_m_impl(k, v));
    }

    /*
     * The private constructor
     */
    pdu_set_m_impl::pdu_set_m_impl(pmt::pmt_t k, pmt::pmt_t v)
      : gr::block("pdu_set_m",
              io_signature::make (0, 0, 0),
         		  io_signature::make (0, 0, 0)),
        d_k(k),
        d_v(v)
    {
      message_port_register_in(PMTCONSTSTR__PDU_IN);
      set_msg_handler(PMTCONSTSTR__PDU_IN,
          boost::bind(&pdu_set_m_impl::handle_msg, this, _1));
      message_port_register_in(PMTCONSTSTR__CTRL);
      set_msg_handler(PMTCONSTSTR__CTRL, boost::bind(&pdu_set_m_impl::handle_ctrl_msg, this, _1));
      message_port_register_out(PMTCONSTSTR__PDU_OUT);
    }

    /*
     * Our virtual destructor.
     */
    pdu_set_m_impl::~pdu_set_m_impl()
    {
    }


    void
    pdu_set_m_impl::setup_rpc()
    {
      #ifdef GR_CTRLPORT

      add_rpc_variable(
          rpcbasic_sptr(new rpcbasic_register_handler<pdu_set_m>(
              alias(), "ctrl",
              "", "ctrl",
              RPC_PRIVLVL_MIN, DISPNULL)));

      #endif /* GR_CTRLPORT */
    }


    /*
     * The following handler is responsible for accepting commands to set the
     * various controllable variables using their public methods.
     */
    void
    pdu_set_m_impl::handle_ctrl_msg(pmt::pmt_t msg)
    {
      gr::thread::scoped_lock l(d_setlock);

      if(pmt::is_pair(msg)) {
        if(pmt::eqv(pmt::car(msg), pmt::mp("val"))) {
          GR_LOG_NOTICE(d_logger, boost::format("value is %1%") % val());
        } else if(pmt::eqv(pmt::car(msg), pmt::mp("key"))) {
          GR_LOG_NOTICE(d_logger, boost::format("key is %1%") % key());
        } else if(pmt::eqv(pmt::car(msg), pmt::mp("set_val"))) {
          set_val(pmt::cdr(msg));
          GR_LOG_NOTICE(d_logger, boost::format("value set to %1%") % val());
        } else if(pmt::eqv(pmt::car(msg), pmt::mp("set_key"))) {
          set_key(pmt::cdr(msg));
          GR_LOG_DEBUG(d_logger, boost::format("key set to %1%") % key());
        } else {
          GR_LOG_WARN(d_logger, boost::format("invalid command %1% received...") % pmt::car(msg));
        }
      } else {
        GR_LOG_WARN(d_logger, "received unexpected PMT command (non-pair)");
      }
    }


    void
    pdu_set_m_impl::handle_msg(pmt::pmt_t pdu)
    {
      // make sure PDU data is formed properly
      if (!(pmt::is_pair(pdu))) {
        GR_LOG_WARN(d_logger, "received unexpected PMT (non-pair)");
        return;
      }

      gr::thread::scoped_lock l(d_setlock);

      /* code */
      pmt::pmt_t meta = pmt::car(pdu);


      if (pmt::is_dict(meta)) {
        meta = pmt::dict_add(meta, d_k, d_v);
        message_port_pub(PMTCONSTSTR__PDU_OUT, pmt::cons(meta, pmt::cdr(pdu)));
      } else {
        GR_LOG_WARN(d_logger, "received malformed PDU");
      }
    }


    void
    pdu_set_m_impl::set_key(pmt::pmt_t k)
    {
      gr::thread::scoped_lock l(d_setlock);

      d_k = k;
    }


    void
    pdu_set_m_impl::set_val(pmt::pmt_t v)
    {
      gr::thread::scoped_lock l(d_setlock);

      d_v = v;
    }


    void
    pdu_set_m_impl::set_kv(pmt::pmt_t k, pmt::pmt_t v)
    {
      gr::thread::scoped_lock l(d_setlock);

      d_k = k;
      d_v = v;
    }

  } /* namespace pdu_utils */
} /* namespace gr */
