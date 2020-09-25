#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright 2018, 2019, 2020 National Technology & Engineering Solutions of Sandia, LLC
# (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
# retains certain rights in this software.
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

import pdu_utils_swig as pdu_utils
from gnuradio import gr, gr_unittest
from gnuradio import blocks
import pmt
import time


class qa_pdu_preamble (gr_unittest.TestCase):

    def setUp(self):
        self.tb = gr.top_block()
        self.emitter = pdu_utils.message_emitter()
        self.debug = blocks.message_debug()

    def connectUp(self):
        self.tb.msg_connect((self.emitter, 'msg'), (self.dut, 'pdu_in'))
        self.tb.msg_connect((self.dut, 'pdu_out'), (self.debug, 'store'))

    def tearDown(self):
        self.tb = None

    def test_001_combo(self):

        self.dut = pdu_utils.pdu_preamble([0, 1, 0, 1], [], 2, 3, True)
        self.connectUp()

        input_data = pmt.init_u8vector(8, [1, 0, 1, 1, 0, 0, 1, 0])
        input_dict = pmt.dict_add(pmt.make_dict(), pmt.intern("KEY"), pmt.intern("VALUE"))
        input_pdu = pmt.cons(input_dict, input_data)

        expected_data = [0, 0, 0, -1, -1, 1, 1, -1, -1, 1, 1, 1, 1, -1, -1, 1, 1, 1, 1, -1, -1, -1, -1, 1, 1, -1, -1, 0, 0, 0]
        expected_dict = pmt.dict_add(pmt.make_dict(), pmt.intern("KEY"), pmt.intern("VALUE"))
        expected_pdu = pmt.cons(expected_dict, pmt.init_f32vector(len(expected_data), expected_data))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(pmt.intern("MALFORMED PDU"))
        time.sleep(.001)
        self.emitter.emit(input_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        self.assertEqual(1, self.debug.num_messages())

        print("test_001_basic_nrz_test:")
        print("pdu expected: " + repr(pmt.car(expected_pdu)))
        print("pdu got:      " + repr(pmt.car(self.debug.get_message(0))))
        print("data expected: " + repr(pmt.to_python(pmt.cdr(expected_pdu))))
        print("data got:      " + repr(pmt.to_python(pmt.cdr(self.debug.get_message(0)))))
        print

        self.assertTrue(pmt.equal(self.debug.get_message(0), expected_pdu))

    def test_002_basic_head(self):

        self.dut = pdu_utils.pdu_preamble([1, 0, 1, 0], [], 1, 0, False)
        self.connectUp()

        input_data = [1, 1, 0, 0, 1, 1, 0, 0]
        input_dict = pmt.dict_add(pmt.make_dict(), pmt.intern("KEY"), pmt.intern("VALUE"))
        input_pdu = pmt.cons(input_dict, pmt.init_u8vector(len(input_data), input_data))

        expected_data = [1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0]
        expected_dict = pmt.dict_add(pmt.make_dict(), pmt.intern("KEY"), pmt.intern("VALUE"))
        expected_pdu = pmt.cons(expected_dict, pmt.init_f32vector(len(expected_data), expected_data))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(input_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        self.assertEqual(1, self.debug.num_messages())

        print("test_002_basic_head:")
        print("pdu expected: " + repr(pmt.car(expected_pdu)))
        print("pdu got:      " + repr(pmt.car(self.debug.get_message(0))))
        print("data expected: " + repr(pmt.to_python(pmt.cdr(expected_pdu))))
        print("data got:      " + repr(pmt.to_python(pmt.cdr(self.debug.get_message(0)))))
        print

        self.assertTrue(pmt.equal(self.debug.get_message(0), expected_pdu))

    def test_003_basic_tail(self):

        self.dut = pdu_utils.pdu_preamble([], [1, 0, 1, 0], 1, 0, False)
        self.connectUp()

        input_data = [1, 1, 0, 0, 1, 1, 0, 0]
        input_dict = pmt.dict_add(pmt.make_dict(), pmt.intern("KEY"), pmt.intern("VALUE"))
        input_pdu = pmt.cons(input_dict, pmt.init_u8vector(len(input_data), input_data))

        expected_data = [1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0]
        expected_dict = pmt.dict_add(pmt.make_dict(), pmt.intern("KEY"), pmt.intern("VALUE"))
        expected_pdu = pmt.cons(expected_dict, pmt.init_f32vector(len(expected_data), expected_data))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(input_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        self.assertEqual(1, self.debug.num_messages())

        print("test_003_basic_tail:")
        print("pdu expected: " + repr(pmt.car(expected_pdu)))
        print("pdu got:      " + repr(pmt.car(self.debug.get_message(0))))
        print("data expected: " + repr(pmt.to_python(pmt.cdr(expected_pdu))))
        print("data got:      " + repr(pmt.to_python(pmt.cdr(self.debug.get_message(0)))))
        print

        self.assertTrue(pmt.equal(self.debug.get_message(0), expected_pdu))

    def test_004_basic_interp(self):

        self.dut = pdu_utils.pdu_preamble([], [], 2, 0, False)
        self.connectUp()

        input_data = [1, 1, 0, 0, 1, 1, 0, 0]
        input_dict = pmt.dict_add(pmt.make_dict(), pmt.intern("KEY"), pmt.intern("VALUE"))
        input_pdu = pmt.cons(input_dict, pmt.init_u8vector(len(input_data), input_data))

        expected_data = [1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0]
        expected_dict = pmt.dict_add(pmt.make_dict(), pmt.intern("KEY"), pmt.intern("VALUE"))
        expected_pdu = pmt.cons(expected_dict, pmt.init_f32vector(len(expected_data), expected_data))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(input_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        self.assertEqual(1, self.debug.num_messages())

        print("test_004_basic_interp:")
        print("pdu expected: " + repr(pmt.car(expected_pdu)))
        print("pdu got:      " + repr(pmt.car(self.debug.get_message(0))))
        print("data expected: " + repr(pmt.to_python(pmt.cdr(expected_pdu))))
        print("data got:      " + repr(pmt.to_python(pmt.cdr(self.debug.get_message(0)))))
        print

        self.assertTrue(pmt.equal(self.debug.get_message(0), expected_pdu))

    def test_005_basic_zero(self):

        self.dut = pdu_utils.pdu_preamble([], [], 1, 5, False)
        self.connectUp()

        input_data = [1, 1, 1, 1]
        input_dict = pmt.dict_add(pmt.make_dict(), pmt.intern("KEY"), pmt.intern("VALUE"))
        input_pdu = pmt.cons(input_dict, pmt.init_u8vector(len(input_data), input_data))

        expected_data = [0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0]
        expected_dict = pmt.dict_add(pmt.make_dict(), pmt.intern("KEY"), pmt.intern("VALUE"))
        expected_pdu = pmt.cons(expected_dict, pmt.init_f32vector(len(expected_data), expected_data))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(input_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        self.assertEqual(1, self.debug.num_messages())

        print("test_005_basic_zero:")
        print("pdu expected: " + repr(pmt.car(expected_pdu)))
        print("pdu got:      " + repr(pmt.car(self.debug.get_message(0))))
        print("data expected: " + repr(pmt.to_python(pmt.cdr(expected_pdu))))
        print("data got:      " + repr(pmt.to_python(pmt.cdr(self.debug.get_message(0)))))
        print

        self.assertTrue(pmt.equal(self.debug.get_message(0), expected_pdu))

    def test_006_basic_nrz(self):

        self.dut = pdu_utils.pdu_preamble([], [], 1, 0, True)
        self.connectUp()

        input_data = pmt.init_u8vector(8, [1, 0, 1, 1, 0, 0, 1, 0])
        input_dict = pmt.dict_add(pmt.make_dict(), pmt.intern("KEY"), pmt.intern("VALUE"))
        input_pdu = pmt.cons(input_dict, input_data)

        expected_data = [1, -1, 1, 1, -1, -1, 1, -1]
        expected_dict = pmt.dict_add(pmt.make_dict(), pmt.intern("KEY"), pmt.intern("VALUE"))
        expected_pdu = pmt.cons(expected_dict, pmt.init_f32vector(len(expected_data), expected_data))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(input_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        self.assertEqual(1, self.debug.num_messages())

        print("test_006_basic_nrz:")
        print("pdu expected: " + repr(pmt.car(expected_pdu)))
        print("pdu got:      " + repr(pmt.car(self.debug.get_message(0))))
        print("data expected: " + repr(pmt.to_python(pmt.cdr(expected_pdu))))
        print("data got:      " + repr(pmt.to_python(pmt.cdr(self.debug.get_message(0)))))
        print

        self.assertTrue(pmt.equal(self.debug.get_message(0), expected_pdu))


"""
Tests to add:
 - test callbacks
 - test Tail
 - test error conditions
 - test alternate inputs
"""

if __name__ == '__main__':
    gr_unittest.run(qa_pdu_preamble)
