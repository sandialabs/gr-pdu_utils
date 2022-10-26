#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright 2018, 2019, 2020 National Technology & Engineering Solutions of Sandia, LLC
# (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
# retains certain rights in this software.
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

import numpy
from gnuradio import gr
from datetime import datetime
import time
import pmt
from threading import Timer
from gnuradio import pdu_utils

class pdu_delay(gr.basic_block):
    """
    PDU Delay

    This block receives a PDU and delays it by a defined amount of time.
    The delay is user-defined (in seconds)  and is measured from the time
    the block receives the PDU to the time it outputs the PDU.

    A debug mode can be enabled; if the PDU is a pair with a dict datatype,
    the block will append two key/value pairs to the dict: the time the PDU
    entered the block with the key "time_in", and the time the PDU exited
    the block with the key "time_out".

    The block also accepts a delay value defined by the PDU. If the PDU has
    a dict key "delay" with a non-negative double as its value, the block
    will delay the PDU by that value instead.
    """
    def __init__(self, delay):
        gr.basic_block.__init__(self,
            name="pdu_delay",
            in_sig=None,
            out_sig=None)

        self.log = gr.logger('log')

        self.set_delay(delay)
        self.debug = False

        self.message_port_register_in( pdu_utils.PMTCONSTSTR__pdu_in() )
        self.set_msg_handler( pdu_utils.PMTCONSTSTR__pdu_in(), self.handle_pdu)
        self.message_port_register_out( pdu_utils.PMTCONSTSTR__pdu_out() )

        # pre-define PDU keys
        self.in_key = pmt.intern("time_in")
        self.out_key = pmt.intern("time_out")
        self.delay_key = pmt.intern("delay")

    # define the delay time
    def set_delay(self, delay):
        # throw error if delay is negative
        if delay >= 0:
            self.delay = delay
        else:
            self.log.error("Delay must be non-negative")
            raise RuntimeError("")

    # enable/disable debug mode
    def set_debug(self, debug):
        self.debug = debug

    # receive PDU
    def handle_pdu(self, pdu):
        # if PDU is not pair, drop and wait for new PDU
        if not pmt.is_pair(pdu):
            self.log.warn("PDU is not a pair, dropping")
            return
        meta = pmt.car(pdu)
        # set default delay to internal value
        delay = self.delay
        # if PDU has delay in the metadata, try to use that value first
        if pmt.is_dict(meta) and pmt.dict_has_key(meta, self.delay_key):
            try:
                delay = pmt.to_double(pmt.dict_ref(meta, self.delay_key, pmt.from_double(self.delay)))
                if delay < 0:
                    self.log.warn("PDU delay is negative, using default delay")
                    delay = self.delay
            except:
                self.log.warn("Invalid delay from PDU metadata, using default delay")
                delay = self.delay
        # if debug mode is enabled, append input time to metadata
        if pmt.is_dict(meta) and self.debug:
            meta = pmt.dict_add(meta, self.in_key, pmt.from_double(time.time()))
            pdu = pmt.cons(meta, pmt.cdr(pdu))
        # set timer to trigger on_timer
        t = Timer(delay, self.on_timer, (pdu,))
        t.start()

    # output PDU after delay
    def on_timer(self, pdu):
        # debug mode: append output time to metadata
        meta = pmt.car(pdu)
        if pmt.is_dict(meta) and self.debug:
            meta = pmt.dict_add(meta, self.out_key, pmt.from_double(time.time()))
            pdu = pmt.cons(meta, pmt.cdr(pdu))
        # release PDU
        self.message_port_pub( pdu_utils.PMTCONSTSTR__pdu_out(), pdu)

    def forecast(self, noutput_items, ninput_items_required):
        pass

    def general_work(self, input_items, output_items):
        pass
