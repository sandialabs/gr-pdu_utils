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
    import pdu_utils
except ImportError:
    import os
    import sys
    dirname, filename = os.path.split(os.path.abspath(__file__))
    sys.path.append(os.path.join(dirname, "bindings"))
    import pdu_utils
from gnuradio import gr, gr_unittest
from gnuradio import blocks
import pmt
import time


class qa_pdu_pfb_resamp (gr_unittest.TestCase):

    def setUp(self):
        self.tb = gr.top_block()
        self.emitter = pdu_utils.message_emitter()
        self.debug = blocks.message_debug()

    def connectUp(self):
        self.tb.msg_connect((self.emitter, 'msg'), (self.dut, 'pdu_in'))
        self.tb.msg_connect((self.dut, 'pdu_out'), (self.debug, 'store'))

    def tearDown(self):
        self.tb = None

    def test_001_instantiation_fff(self):
        self.dut = pdu_utils.pdu_pfb_resamp_fff([1], 1, 1.0)
        self.connectUp()

        self.tb.start()
        time.sleep(.5)
        self.tb.stop()
        self.tb.wait()

        self.assertTrue(True)

    def test_002_instantiation_ccf(self):
        self.dut = pdu_utils.pdu_pfb_resamp_ccf([1], 1, 1.0)
        self.connectUp()

        self.tb.start()
        time.sleep(.5)
        self.tb.stop()
        self.tb.wait()

        self.assertTrue(True)

    def test_003_instantiation_ccc(self):
        self.dut = pdu_utils.pdu_pfb_resamp_ccc([1], 1, 1.0)
        self.connectUp()

        self.tb.start()
        time.sleep(.5)
        self.tb.stop()
        self.tb.wait()

        self.assertTrue(True)

    def test_004_fff_straight(self):
        self.dut = pdu_utils.pdu_pfb_resamp_fff([1], 1, 1.0)
        self.connectUp()

        in_data = [1, 1, 0, 0] * 4
        in_pdu = pmt.cons(pmt.make_dict(), pmt.init_f32vector(len(in_data), in_data))

        expected_data = [1, 1, 0, 0] * 4
        expected_pdu = pmt.cons(pmt.make_dict(), pmt.init_f32vector(len(expected_data), expected_data))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        self.assertEqual(1, self.debug.num_messages())

        print("test_004_fff_straight:")
        print("pdu expected: " + repr(pmt.car(expected_pdu)))
        print("pdu got:      " + repr(pmt.car(self.debug.get_message(0))))
        print("data expected: " + repr(pmt.to_python(pmt.cdr(expected_pdu))))
        print("data got:      " + repr(pmt.to_python(pmt.cdr(self.debug.get_message(0)))))
        print

        self.assertTrue(pmt.equal(self.debug.get_message(0), expected_pdu))

    def test_005_fff_down(self):
        self.dut = pdu_utils.pdu_pfb_resamp_fff([1], 1, 0.5)
        self.connectUp()

        in_data = [1, 1, 0, 0] * 4
        in_pdu = pmt.cons(pmt.make_dict(), pmt.init_f32vector(len(in_data), in_data))

        expected_data = [1, 0] * 4
        expected_pdu = pmt.cons(pmt.make_dict(), pmt.init_f32vector(len(expected_data), expected_data))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        self.assertEqual(1, self.debug.num_messages())

        print("test_005_fff_down:")
        print("pdu expected: " + repr(pmt.car(expected_pdu)))
        print("pdu got:      " + repr(pmt.car(self.debug.get_message(0))))
        print("data expected: " + repr(pmt.to_python(pmt.cdr(expected_pdu))))
        print("data got:      " + repr(pmt.to_python(pmt.cdr(self.debug.get_message(0)))))
        print

        self.assertTrue(pmt.equal(self.debug.get_message(0), expected_pdu))

    def test_006_fff_up(self):
        self.dut = pdu_utils.pdu_pfb_resamp_fff([1], 1, 2.0)
        self.connectUp()

        in_data = [1, 1, 0, 0] * 4
        in_pdu = pmt.cons(pmt.make_dict(), pmt.init_f32vector(len(in_data), in_data))

        expected_data = [1, 1, 1, 1, 0, 0, 0, 0] * 4
        expected_pdu = pmt.cons(pmt.make_dict(), pmt.init_f32vector(len(expected_data), expected_data))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        self.assertEqual(1, self.debug.num_messages())

        print("test_006_fff_up:")
        print("pdu expected: " + repr(pmt.car(expected_pdu)))
        print("pdu got:      " + repr(pmt.car(self.debug.get_message(0))))
        print("data expected: " + repr(pmt.to_python(pmt.cdr(expected_pdu))))
        print("data got:      " + repr(pmt.to_python(pmt.cdr(self.debug.get_message(0)))))
        print

        self.assertTrue(pmt.equal(self.debug.get_message(0), expected_pdu))

    def test_007_fff_gain(self):
        self.dut = pdu_utils.pdu_pfb_resamp_fff([2], 1, 1.0)
        self.connectUp()

        in_data = [1, 1, 0, 0] * 4
        in_pdu = pmt.cons(pmt.make_dict(), pmt.init_f32vector(len(in_data), in_data))

        expected_data = [2, 2, 0, 0] * 4
        expected_pdu = pmt.cons(pmt.make_dict(), pmt.init_f32vector(len(expected_data), expected_data))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        self.assertEqual(1, self.debug.num_messages())

        print("test_007_fff_gain:")
        print("pdu expected: " + repr(pmt.car(expected_pdu)))
        print("pdu got:      " + repr(pmt.car(self.debug.get_message(0))))
        print("data expected: " + repr(pmt.to_python(pmt.cdr(expected_pdu))))
        print("data got:      " + repr(pmt.to_python(pmt.cdr(self.debug.get_message(0)))))
        print

        self.assertTrue(pmt.equal(self.debug.get_message(0), expected_pdu))

    def test_008_fff_attn(self):
        self.dut = pdu_utils.pdu_pfb_resamp_fff([0.5], 1, 1.0)
        self.connectUp()

        in_data = [1, 1, 0, 0] * 4
        in_pdu = pmt.cons(pmt.make_dict(), pmt.init_f32vector(len(in_data), in_data))

        expected_data = [0.5, 0.5, 0, 0] * 4
        expected_pdu = pmt.cons(pmt.make_dict(), pmt.init_f32vector(len(expected_data), expected_data))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        self.assertEqual(1, self.debug.num_messages())

        print("test_007_fff_gain:")
        print("pdu expected: " + repr(pmt.car(expected_pdu)))
        print("pdu got:      " + repr(pmt.car(self.debug.get_message(0))))
        print("data expected: " + repr(pmt.to_python(pmt.cdr(expected_pdu))))
        print("data got:      " + repr(pmt.to_python(pmt.cdr(self.debug.get_message(0)))))
        print

        self.assertTrue(pmt.equal(self.debug.get_message(0), expected_pdu))


if __name__ == '__main__':
    gr_unittest.run(qa_pdu_pfb_resamp)
