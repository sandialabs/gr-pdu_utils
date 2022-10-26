#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright 2018, 2019, 2020 National Technology & Engineering Solutions of Sandia, LLC
# (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
# retains certain rights in this software.
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

from gnuradio import gr, gr_unittest
from gnuradio import blocks
from gnuradio import pdu_utils
import pmt
import time
from threading import Timer
import numpy as np

class qa_pdu_delay(gr_unittest.TestCase):

    def setUp(self):
        self.tb = gr.top_block()
        self.emitter = pdu_utils.message_emitter()
        self.debug = blocks.message_debug()
        self.delay = pdu_utils.pdu_delay(1.0)
        self.delay.set_debug(True)

        self.tb.msg_connect((self.emitter, 'msg'), (self.delay, 'pdu_in'))
        self.tb.msg_connect((self.delay, 'pdu_out'), (self.debug, 'store'))

        self.input_delays = ()

    def tearDown(self):
        self.tb = None

    # test using a single PDU, verify that delay is correct to 2 decimal places
    def test_001_t(self):
        self.delay.set_delay(0.1)
        i_vec = pmt.init_u8vector(8, [0, 1, 0, 0, 1, 0, 1, 1])
        in_pdu = pmt.cons(pmt.make_dict(), i_vec)
        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(self.delay.delay+0.01)
        self.tb.stop()
        self.tb.wait()

        t0 = pmt.to_double(pmt.dict_ref(pmt.car(self.debug.get_message(0)),
                                                self.delay.in_key,
                                                pmt.make_dict()))
        t1 = pmt.to_double(pmt.dict_ref(pmt.car(self.debug.get_message(0)),
                                                self.delay.out_key,
                                                pmt.make_dict()))
        dt = t1-t0
        self.assertAlmostEqual(dt, self.delay.delay, 2, "")
        self.assertTrue(pmt.equal(pmt.cdr(self.debug.get_message(0)),i_vec))

    # test using multiple PDUs, emitted according to self.input_delays
    # verify delay from input/output to 2 decimal places
    # also, verify delays between PDUs, to 2 decimal places
    def test_002_t(self):
        self.delay.set_delay(0.5)
        self.input_delays = (0.0, 0.3, 0.5, 0.6)
        i_vec = pmt.init_u8vector(8, [0, 1, 0, 0, 1, 0, 1, 1])
        in_pdu = pmt.cons(pmt.make_dict(), i_vec)
        self.tb.start()
        time.sleep(.001)
        for x in self.input_delays:
            time.sleep(x)
            self.emitter.emit(in_pdu)
        time.sleep(self.delay.delay+0.01)
        self.tb.stop()
        self.tb.wait()
        t = np.empty([len(self.input_delays), 2], dtype=float)
        j = 0
        for x in self.input_delays:
            t[j][0] = pmt.to_double(pmt.dict_ref(pmt.car(self.debug.get_message(j)),
                                                                      self.delay.in_key,
                                                                      pmt.make_dict()))
            t[j][1] = pmt.to_double(pmt.dict_ref(pmt.car(self.debug.get_message(j)),
                                                                      self.delay.out_key,
                                                                      pmt.make_dict()))
            j += 1

        j = 0
        for x in self.input_delays[:-1]:
            dt = t[j][1] - t[j][0]
            dti = t[j+1][0] - t[j][0]
            self.assertTrue(pmt.equal(pmt.cdr(self.debug.get_message(j)),i_vec))
            self.assertAlmostEqual(dt, self.delay.delay, 2, "")
            self.assertAlmostEqual(dti, self.input_delays[j+1], 2, "")
            j += 1
        self.assertAlmostEqual(t[j][1] - t[j][0], self.delay.delay, 2, "")


    # verify that a negative default delay value throws an error
    def test_003_t(self):
        try:
            self.delay.set_delay(-1.0)
            self.singlePDU()
            self.assertTrue(False)
        except (RuntimeError):
            self.assertTrue(True)

    # verify that updating the delay from PDU metadata works as intended
    # valid delay, negative delay, and non-float delay
    def test_004_t(self):
        self.delay.set_delay(0.2)
        car_valid = pmt.dict_add(pmt.make_dict(), self.delay.delay_key, pmt.from_double(0.5))
        car_negative = pmt.dict_add(pmt.make_dict(), self.delay.delay_key, pmt.from_double(-0.5))
        car_nonfloat = pmt.dict_add(pmt.make_dict(), self.delay.delay_key, pmt.intern("0.5"))
        i_vec = pmt.init_u8vector(8, [0, 1, 0, 0, 1, 0, 1, 1])
        in_pdu_valid = pmt.cons(car_valid, i_vec)
        in_pdu_negative = pmt.cons(car_negative, i_vec)
        in_pdu_nonfloat = pmt.cons(car_nonfloat, i_vec)

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_pdu_valid)
        time.sleep(.4)
        self.emitter.emit(in_pdu_negative)
        time.sleep(.4)
        self.emitter.emit(in_pdu_nonfloat)
        time.sleep(.21)
        self.tb.stop()
        self.tb.wait()

        t = np.empty([3, 2], dtype=float)
        for j in range(0, 3):
            t[j][0] = pmt.to_double(pmt.dict_ref(pmt.car(self.debug.get_message(j)),
                                                                 self.delay.in_key,
                                                                 pmt.make_dict()))
            t[j][1] = pmt.to_double(pmt.dict_ref(pmt.car(self.debug.get_message(j)),
                                                                 self.delay.out_key,
                                                                 pmt.make_dict()))

        e_dt = (0.5, 0.2, 0.2,)
        e_dti = (0.4, 0.4,)
        for j in range(0, 2):
            dt = t[j][1] - t[j][0]
            dti = t[j+1][0] - t[j][0]
            self.assertTrue(pmt.equal(pmt.cdr(self.debug.get_message(j)),i_vec))
            self.assertAlmostEqual(dt, e_dt[j], 2, "")
            self.assertAlmostEqual(dti, e_dti[j], 2, "")
        self.assertAlmostEqual(t[2][1] - t[2][0], e_dt[2], 2, "")

    # verify that malformed PDUs are handled properly
    def test_005_t(self):
        self.delay.set_delay(0.5)
        self.input_delays = (0.0, 0.3, 0.7,)
        i_vec = pmt.init_u8vector(8, [0, 1, 0, 0, 1, 0, 1, 1])
        in_pdu = pmt.cons(pmt.make_dict(), i_vec)
        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(self.input_delays[1])
        self.emitter.emit(pmt.intern("MALFORMED PDU"))
        time.sleep(self.input_delays[2])
        self.emitter.emit(in_pdu)
        time.sleep(self.delay.delay+0.01)
        self.tb.stop()
        self.tb.wait()
        t = np.empty([2, 2], dtype=float)
        j = 0
        for x in range(0, 2):
            t[j][0] = pmt.to_double(pmt.dict_ref(pmt.car(self.debug.get_message(j)),
                                                         self.delay.in_key,
                                                         pmt.make_dict()))
            t[j][1] = pmt.to_double(pmt.dict_ref(pmt.car(self.debug.get_message(j)),
                                                         self.delay.out_key,
                                                         pmt.make_dict()))
            j += 1

        j = 0
        for x in self.input_delays[::2]:
            dt = t[j][1] - t[j][0]
            self.assertTrue(pmt.equal(pmt.cdr(self.debug.get_message(j)),i_vec))
            self.assertAlmostEqual(dt, self.delay.delay, 2, "")
            j += 1

        dti = t[1][0] - t[0][0]
        self.assertAlmostEqual(dti, self.input_delays[1]+self.input_delays[2], 2, "")

if __name__ == '__main__':
    gr_unittest.run(qa_pdu_delay)
