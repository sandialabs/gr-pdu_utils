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

#include <gnuradio/io_signature.h>
#include <pdu_utils/constants.h>

namespace gr {
namespace pdu_utils {


const pmt::pmt_t PMTCONSTSTR__msg()
{
    static const pmt::pmt_t val = pmt::mp("msg");
    return val;
}
const pmt::pmt_t PMTCONSTSTR__in()
{
    static const pmt::pmt_t val = pmt::mp("in");
    return val;
}
const pmt::pmt_t PMTCONSTSTR__out()
{
    static const pmt::pmt_t val = pmt::mp("out");
    return val;
}
const pmt::pmt_t PMTCONSTSTR__pdu_in()
{
    static const pmt::pmt_t val = pmt::mp("pdu_in");
    return val;
}
const pmt::pmt_t PMTCONSTSTR__pdu_out()
{
    static const pmt::pmt_t val = pmt::mp("pdu_out");
    return val;
}
const pmt::pmt_t PMTCONSTSTR__cpdus()
{
    static const pmt::pmt_t val = pmt::mp("cpdus");
    return val;
}
const pmt::pmt_t PMTCONSTSTR__fpdus()
{
    static const pmt::pmt_t val = pmt::mp("fpdus");
    return val;
}
const pmt::pmt_t PMTCONSTSTR__burst_time()
{
    static const pmt::pmt_t val = pmt::mp("burst_time");
    return val;
}
const pmt::pmt_t PMTCONSTSTR__start_time()
{
    static const pmt::pmt_t val = pmt::mp("start_time");
    return val;
}
const pmt::pmt_t PMTCONSTSTR__start_time_offset()
{
    static const pmt::pmt_t val = pmt::mp("start_time_offset");
    return val;
}
const pmt::pmt_t PMTCONSTSTR__rx_time()
{
    static const pmt::pmt_t val = pmt::mp("rx_time");
    return val;
}
const pmt::pmt_t PMTCONSTSTR__tx_time()
{
    static const pmt::pmt_t val = pmt::mp("tx_time");
    return val;
}
const pmt::pmt_t PMTCONSTSTR__uhd_time_tuple()
{
    static const pmt::pmt_t val = pmt::mp("uhd_time_tuple");
    return val;
}
const pmt::pmt_t PMTCONSTSTR__dict()
{
    static const pmt::pmt_t val = pmt::mp("dict");
    return val;
}
const pmt::pmt_t PMTCONSTSTR__data()
{
    static const pmt::pmt_t val = pmt::mp("data");
    return val;
}
const pmt::pmt_t PMTCONSTSTR__emit()
{
    static const pmt::pmt_t val = pmt::mp("emit");
    return val;
}
const pmt::pmt_t PMTCONSTSTR__head()
{
    static const pmt::pmt_t val = pmt::mp("head");
    return val;
}
const pmt::pmt_t PMTCONSTSTR__tail()
{
    static const pmt::pmt_t val = pmt::mp("tail");
    return val;
}
const pmt::pmt_t PMTCONSTSTR__ctrl()
{
    static const pmt::pmt_t val = pmt::mp("ctrl");
    return val;
}
const pmt::pmt_t PMTCONSTSTR__conf()
{
    static const pmt::pmt_t val = pmt::mp("conf");
    return val;
}
const pmt::pmt_t PMTCONSTSTR__val()
{
    static const pmt::pmt_t val = pmt::mp("val");
    return val;
}
const pmt::pmt_t PMTCONSTSTR__key()
{
    static const pmt::pmt_t val = pmt::mp("key");
    return val;
}
const pmt::pmt_t PMTCONSTSTR__set_val()
{
    static const pmt::pmt_t val = pmt::mp("set_val");
    return val;
}
const pmt::pmt_t PMTCONSTSTR__set_key()
{
    static const pmt::pmt_t val = pmt::mp("set_key");
    return val;
}
const pmt::pmt_t PMTCONSTSTR__bursts()
{
    static const pmt::pmt_t val = pmt::mp("bursts");
    return val;
}
const pmt::pmt_t PMTCONSTSTR__detects()
{
    static const pmt::pmt_t val = pmt::mp("detects");
    return val;
}
const pmt::pmt_t PMTCONSTSTR__debug()
{
    static const pmt::pmt_t val = pmt::mp("debug");
    return val;
}
const pmt::pmt_t PMTCONSTSTR__zeroX()
{
    static const pmt::pmt_t val = pmt::mp("zeroX");
    return val;
}
const pmt::pmt_t PMTCONSTSTR__window()
{
    static const pmt::pmt_t val = pmt::mp("window");
    return val;
}
const pmt::pmt_t PMTCONSTSTR__burst_id()
{
    static const pmt::pmt_t val = pmt::mp("burst_id");
    return val;
}
const pmt::pmt_t PMTCONSTSTR__burst_index()
{
    static const pmt::pmt_t val = pmt::mp("burst_index");
    return val;
}
const pmt::pmt_t PMTCONSTSTR__eob_offset()
{
    static const pmt::pmt_t val = pmt::mp("eob_offset");
    return val;
}
const pmt::pmt_t PMTCONSTSTR__eob_alignment()
{
    static const pmt::pmt_t val = pmt::mp("eob_alignment");
    return val;
}
const pmt::pmt_t PMTCONSTSTR__sample_rate()
{
    static const pmt::pmt_t val = pmt::mp("sample_rate");
    return val;
}
const pmt::pmt_t PMTCONSTSTR__symbol_rate()
{
    static const pmt::pmt_t val = pmt::mp("symbol_rate");
    return val;
}
const pmt::pmt_t PMTCONSTSTR__tx_sob()
{
    static const pmt::pmt_t val = pmt::mp("tx_sob");
    return val;
}
const pmt::pmt_t PMTCONSTSTR__tx_eob()
{
    static const pmt::pmt_t val = pmt::mp("tx_eob");
    return val;
}
const pmt::pmt_t PMTCONSTSTR__pdu_num()
{
    static const pmt::pmt_t val = pmt::mp("pdu_num");
    return val;
}
const pmt::pmt_t PMTCONSTSTR__time_type()
{
    static const pmt::pmt_t val = pmt::mp("time_type");
    return val;
}
const pmt::pmt_t PMTCONSTSTR__wall_clock_time()
{
    static const pmt::pmt_t val = pmt::mp("wall_clock_time");
    return val;
}
const pmt::pmt_t PMTCONSTSTR__duration()
{
    static const pmt::pmt_t val = pmt::mp("duration");
    return val;
}
const pmt::pmt_t PMTCONSTSTR__set_trigger_tag()
{
    static const pmt::pmt_t val = pmt::mp("set_trigger_tag");
    return val;
}
const pmt::pmt_t PMTCONSTSTR__set_arming_tag()
{
    static const pmt::pmt_t val = pmt::mp("set_arming_tag");
    return val;
}
const pmt::pmt_t PMTCONSTSTR__set_tx_limit()
{
    static const pmt::pmt_t val = pmt::mp("set_tx_limit");
    return val;
}
const pmt::pmt_t PMTCONSTSTR__set_delays()
{
    static const pmt::pmt_t val = pmt::mp("set_delays");
    return val;
}
const pmt::pmt_t PMTCONSTSTR__set_message()
{
    static const pmt::pmt_t val = pmt::mp("set_message");
    return val;
}
const pmt::pmt_t PMTCONSTSTR__set_holdoff()
{
    static const pmt::pmt_t val = pmt::mp("set_holdoff");
    return val;
}
const pmt::pmt_t PMTCONSTSTR__set_armed()
{
    static const pmt::pmt_t val = pmt::mp("set_armed");
    return val;
}
const pmt::pmt_t PMTCONSTSTR__trigger_now()
{
    static const pmt::pmt_t val = pmt::mp("trigger_now");
    return val;
}
const pmt::pmt_t PMTCONSTSTR__system()
{
    static const pmt::pmt_t val = pmt::mp("system");
    return val;
}
const pmt::pmt_t PMTCONSTSTR__phase_inc()
{
    static const pmt::pmt_t val = pmt::mp("phase_inc");
    return val;
}


} /* namespace pdu_utils */
} /* namespace gr */
