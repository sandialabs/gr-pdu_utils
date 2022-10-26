#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright 2018-2021 National Technology & Engineering Solutions of Sandia, LLC
# (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
# retains certain rights in this software.
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

try:
  from gnuradio import pdu_utils
except ImportError:
    import os
    import sys
    dirname, filename = os.path.split(os.path.abspath(__file__))
    sys.path.append(os.path.join(dirname, "bindings"))
    from gnuradio import pdu_utils

from gnuradio import gr, gr_unittest
from gnuradio import blocks
import pmt
import time

class qa_extract_metadata (gr_unittest.TestCase):

    def setUp (self):
        self.tb = None

    def tearDown (self):
        self.tb = None

    def test_interned_string_constants (self):
        assert(pmt.eq(pdu_utils.PMTCONSTSTR__pdu_in(), pmt.intern("pdu_in")))
        assert(pmt.eq(pdu_utils.PMTCONSTSTR__msg(), pmt.intern("msg")))
        assert(pmt.eq(pdu_utils.PMTCONSTSTR__in(), pmt.intern("in")))
        assert(pmt.eq(pdu_utils.PMTCONSTSTR__out(), pmt.intern("out")))
        assert(pmt.eq(pdu_utils.PMTCONSTSTR__pdu_in(), pmt.intern("pdu_in")))
        assert(pmt.eq(pdu_utils.PMTCONSTSTR__pdu_out(), pmt.intern("pdu_out")))
        assert(pmt.eq(pdu_utils.PMTCONSTSTR__cpdus(), pmt.intern("cpdus")))
        assert(pmt.eq(pdu_utils.PMTCONSTSTR__fpdus(), pmt.intern("fpdus")))
        assert(pmt.eq(pdu_utils.PMTCONSTSTR__burst_time(), pmt.intern("burst_time")))
        assert(pmt.eq(pdu_utils.PMTCONSTSTR__start_time(), pmt.intern("start_time")))
        assert(pmt.eq(pdu_utils.PMTCONSTSTR__start_time_offset(), pmt.intern("start_time_offset")))
        assert(pmt.eq(pdu_utils.PMTCONSTSTR__rx_time(), pmt.intern("rx_time")))
        assert(pmt.eq(pdu_utils.PMTCONSTSTR__tx_time(), pmt.intern("tx_time")))
        assert(pmt.eq(pdu_utils.PMTCONSTSTR__uhd_time_tuple(), pmt.intern("uhd_time_tuple")))
        assert(pmt.eq(pdu_utils.PMTCONSTSTR__dict(), pmt.intern("dict")))
        assert(pmt.eq(pdu_utils.PMTCONSTSTR__data(), pmt.intern("data")))
        assert(pmt.eq(pdu_utils.PMTCONSTSTR__emit(), pmt.intern("emit")))
        assert(pmt.eq(pdu_utils.PMTCONSTSTR__head(), pmt.intern("head")))
        assert(pmt.eq(pdu_utils.PMTCONSTSTR__tail(), pmt.intern("tail")))
        assert(pmt.eq(pdu_utils.PMTCONSTSTR__ctrl(), pmt.intern("ctrl")))
        assert(pmt.eq(pdu_utils.PMTCONSTSTR__conf(), pmt.intern("conf")))
        assert(pmt.eq(pdu_utils.PMTCONSTSTR__val(), pmt.intern("val")))
        assert(pmt.eq(pdu_utils.PMTCONSTSTR__key(), pmt.intern("key")))
        assert(pmt.eq(pdu_utils.PMTCONSTSTR__set_val(), pmt.intern("set_val")))
        assert(pmt.eq(pdu_utils.PMTCONSTSTR__set_key(), pmt.intern("set_key")))
        assert(pmt.eq(pdu_utils.PMTCONSTSTR__bursts(), pmt.intern("bursts")))
        assert(pmt.eq(pdu_utils.PMTCONSTSTR__detects(), pmt.intern("detects")))
        assert(pmt.eq(pdu_utils.PMTCONSTSTR__debug(), pmt.intern("debug")))
        assert(pmt.eq(pdu_utils.PMTCONSTSTR__zeroX(), pmt.intern("zeroX")))
        assert(pmt.eq(pdu_utils.PMTCONSTSTR__window(), pmt.intern("window")))
        assert(pmt.eq(pdu_utils.PMTCONSTSTR__burst_id(), pmt.intern("burst_id")))
        assert(pmt.eq(pdu_utils.PMTCONSTSTR__burst_index(), pmt.intern("burst_index")))
        assert(pmt.eq(pdu_utils.PMTCONSTSTR__eob_offset(), pmt.intern("eob_offset")))
        assert(pmt.eq(pdu_utils.PMTCONSTSTR__eob_alignment(), pmt.intern("eob_alignment")))
        assert(pmt.eq(pdu_utils.PMTCONSTSTR__sample_rate(), pmt.intern("sample_rate")))
        assert(pmt.eq(pdu_utils.PMTCONSTSTR__symbol_rate(), pmt.intern("symbol_rate")))
        assert(pmt.eq(pdu_utils.PMTCONSTSTR__tx_sob(), pmt.intern("tx_sob")))
        assert(pmt.eq(pdu_utils.PMTCONSTSTR__tx_eob(), pmt.intern("tx_eob")))
        assert(pmt.eq(pdu_utils.PMTCONSTSTR__pdu_num(), pmt.intern("pdu_num")))
        assert(pmt.eq(pdu_utils.PMTCONSTSTR__time_type(), pmt.intern("time_type")))
        assert(pmt.eq(pdu_utils.PMTCONSTSTR__wall_clock_time(), pmt.intern("wall_clock_time")))
        assert(pmt.eq(pdu_utils.PMTCONSTSTR__duration(), pmt.intern("duration")))
        assert(pmt.eq(pdu_utils.PMTCONSTSTR__set_trigger_tag(), pmt.intern("set_trigger_tag")))
        assert(pmt.eq(pdu_utils.PMTCONSTSTR__set_arming_tag(), pmt.intern("set_arming_tag")))
        assert(pmt.eq(pdu_utils.PMTCONSTSTR__set_tx_limit(), pmt.intern("set_tx_limit")))
        assert(pmt.eq(pdu_utils.PMTCONSTSTR__set_delays(), pmt.intern("set_delays")))
        assert(pmt.eq(pdu_utils.PMTCONSTSTR__set_message(), pmt.intern("set_message")))
        assert(pmt.eq(pdu_utils.PMTCONSTSTR__set_holdoff(), pmt.intern("set_holdoff")))
        assert(pmt.eq(pdu_utils.PMTCONSTSTR__set_armed(), pmt.intern("set_armed")))
        assert(pmt.eq(pdu_utils.PMTCONSTSTR__trigger_now(), pmt.intern("trigger_now")))
        assert(pmt.eq(pdu_utils.PMTCONSTSTR__system(), pmt.intern("system")))


if __name__ == '__main__':
    gr_unittest.run(qa_extract_metadata)
