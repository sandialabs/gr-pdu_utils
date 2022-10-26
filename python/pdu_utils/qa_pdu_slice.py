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
try:
  from gnuradio import pdu_utils
except ImportError:
    import os
    import sys
    dirname, filename = os.path.split(os.path.abspath(__file__))
    sys.path.append(os.path.join(dirname, "bindings"))
    from gnuradio import pdu_utils

import pmt
import time

class qa_pdu_slice(gr_unittest.TestCase):

    def setUp(self):
        self.tb = gr.top_block()
        self.emitter = pdu_utils.message_emitter()
        self.debug = blocks.message_debug()

    def connectUp(self):
        self.tb.msg_connect((self.emitter, 'msg'), (self.slice, 'pdu_in'))
        self.tb.msg_connect((self.slice, 'pdu_out'), (self.debug, 'store'))

    def tearDown(self):
        self.tb = None

    def runTest(self, i_vec, e_vec):
        in_pdu = pmt.cons(pmt.make_dict(), i_vec)
        e_pdu = pmt.cons(pmt.make_dict(), e_vec)

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        if pmt.is_u8vector(i_vec):
            print("test: ")
            print("data expected: " + repr(pmt.u8vector_elements(pmt.cdr(e_pdu))))
            print("data received: " + repr(pmt.u8vector_elements(pmt.cdr(self.debug.get_message(0)))))
            print
        elif pmt.is_f32vector(i_vec):
            print("test: ")
            print("data expected: " + repr(pmt.f32vector_elements(pmt.cdr(e_pdu))))
            print("data received: " + repr(pmt.f32vector_elements(pmt.cdr(self.debug.get_message(0)))))
            print
        elif pmt.is_c32vector(i_vec):
            print("test: ")
            print("data expected: " + repr(pmt.c32vector_elements(pmt.cdr(e_pdu))))
            print("data received: " + repr(pmt.c32vector_elements(pmt.cdr(self.debug.get_message(0)))))
            print
        else:
            self.assertTrue(False)

        self.assertTrue(pmt.equal(self.debug.get_message(0), e_pdu))

    def runNegativeTest(self, input):
        try:
            self.slice.set_slice(input)
            i_vec = pmt.init_u8vector(1, [0])
            e_vec = pmt.init_u8vector(1, [0])
            in_pdu = pmt.cons(pmt.make_dict(), i_vec)
            e_pdu = pmt.cons(pmt.make_dict(), e_vec)
            self.tb.start()
            time.sleep(.001)
            self.emitter.emit(in_pdu)
            time.sleep(.01)
            self.tb.stop()
            self.tb.wait()
            print("Negative test: data received: " + repr(pmt.u8vector_elements(pmt.cdr(self.debug.get_message(0)))))
            self.assertTrue(False)
        except (RuntimeError):
            self.assertTrue(True)

    # Single-colon positive tests
    def test_001_t(self):
        self.slice = pdu_utils.pdu_slice('[:]')
        self.connectUp()
        i_vec = pmt.init_u8vector(10, [0, 1, 2, 3, 4, 5, 6, 7, 8, 9])
        e_vec = pmt.init_u8vector(10, [0, 1, 2, 3, 4, 5, 6, 7, 8, 9])
        self.runTest(i_vec, e_vec)

    def test_002_t(self):
        self.slice = pdu_utils.pdu_slice('[:]')
        self.connectUp()
        self.slice.set_slice('[3:]')
        i_vec = pmt.init_u8vector(10, [0, 1, 2, 3, 4, 5, 6, 7, 8, 9])
        e_vec = pmt.init_u8vector(7, [3, 4, 5, 6, 7, 8, 9])
        self.runTest(i_vec, e_vec)

    def test_003_t(self):
        self.slice = pdu_utils.pdu_slice('[-8:8]')
        self.slice.set_slice('[-12:6]')
        self.connectUp()
        i_vec = pmt.init_u8vector(10, [0, 1, 2, 3, 4, 5, 6, 7, 8, 9])
        e_vec = pmt.init_u8vector(6, [0, 1, 2, 3, 4, 5])
        self.runTest(i_vec, e_vec)

    def test_004_t(self):
        self.slice = pdu_utils.pdu_slice('[0:-15]')
        self.connectUp()
        i_vec = pmt.init_u8vector(10, [0, 1, 2, 3, 4, 5, 6, 7, 8, 9])
        e_vec = pmt.init_u8vector(0, [])
        self.runTest(i_vec, e_vec)

    def test_005_t(self):
        self.slice = pdu_utils.pdu_slice('[:-6]')
        self.connectUp()
        i_vec = pmt.init_u8vector(10, [0, 1, 2, 3, 4, 5, 6, 7, 8, 9])
        e_vec = pmt.init_u8vector(4, [0, 1, 2, 3])
        self.runTest(i_vec, e_vec)

    def test_006_t(self):
        self.slice = pdu_utils.pdu_slice('[18:0]')
        self.connectUp()
        i_vec = pmt.init_u8vector(10, [0, 1, 2, 3, 4, 5, 6, 7, 8, 9])
        e_vec = pmt.init_u8vector(0, [])
        self.runTest(i_vec, e_vec)

    def test_007_t(self):
        self.slice = pdu_utils.pdu_slice('[-5:12]')
        self.connectUp()
        i_vec = pmt.init_u8vector(10, [0, 1, 2, 3, 4, 5, 6, 7, 8, 9])
        e_vec = pmt.init_u8vector(5, [5, 6, 7, 8, 9])
        self.runTest(i_vec, e_vec)

    def test_008_t(self):
        self.slice = pdu_utils.pdu_slice('[2:7]')
        self.connectUp()
        i_vec = pmt.init_u8vector(10, [0, 1, 2, 3, 4, 5, 6, 7, 8, 9])
        e_vec = pmt.init_u8vector(5, [2, 3, 4, 5, 6])
        self.runTest(i_vec, e_vec)

    def test_009_t(self):
        self.slice = pdu_utils.pdu_slice('[-5:5]')
        self.connectUp()
        i_vec = pmt.init_u8vector(0, [])
        e_vec = pmt.init_u8vector(0, [])
        self.runTest(i_vec, e_vec)

    # Double-colon positive tests
    def test_010_t(self):
        self.slice = pdu_utils.pdu_slice('[::]')
        self.connectUp()
        i_vec = pmt.init_u8vector(10, [0, 1, 2, 3, 4, 5, 6, 7, 8, 9])
        e_vec = pmt.init_u8vector(10, [0, 1, 2, 3, 4, 5, 6, 7, 8, 9])
        self.runTest(i_vec, e_vec)

    def test_011_t(self):
        self.slice = pdu_utils.pdu_slice('[::-1]')
        self.connectUp()
        i_vec = pmt.init_u8vector(10, [0, 1, 2, 3, 4, 5, 6, 7, 8, 9])
        e_vec = pmt.init_u8vector(10, [9, 8, 7, 6, 5, 4, 3, 2, 1, 0])
        self.runTest(i_vec, e_vec)

    def test_012_t(self):
        self.slice = pdu_utils.pdu_slice('[1:8:2]')
        self.connectUp()
        i_vec = pmt.init_u8vector(10, [0, 1, 2, 3, 4, 5, 6, 7, 8, 9])
        e_vec = pmt.init_u8vector(4, [1, 3, 5, 7])
        self.runTest(i_vec, e_vec)

    def test_013_t(self):
        self.slice = pdu_utils.pdu_slice('[1:8:20000]')
        self.connectUp()
        i_vec = pmt.init_u8vector(10, [0, 1, 2, 3, 4, 5, 6, 7, 8, 9])
        e_vec = pmt.init_u8vector(1, [1])
        self.runTest(i_vec, e_vec)

    def test_014_t(self):
        self.slice = pdu_utils.pdu_slice('[-1:-8:-20000]')
        self.connectUp()
        i_vec = pmt.init_u8vector(10, [0, 1, 2, 3, 4, 5, 6, 7, 8, 9])
        e_vec = pmt.init_u8vector(1, [9])
        self.runTest(i_vec, e_vec)

    def test_015_t(self):
        self.slice = pdu_utils.pdu_slice('[-15:-3:3]')
        self.connectUp()
        i_vec = pmt.init_u8vector(10, [0, 1, 2, 3, 4, 5, 6, 7, 8, 9])
        e_vec = pmt.init_u8vector(3, [0, 3, 6])
        self.runTest(i_vec, e_vec)

    def test_016_t(self):
        self.slice = pdu_utils.pdu_slice('[-2::-2]')
        self.connectUp()
        i_vec = pmt.init_u8vector(10, [0, 1, 2, 3, 4, 5, 6, 7, 8, 9])
        e_vec = pmt.init_u8vector(5, [8, 6, 4, 2, 0])
        self.runTest(i_vec, e_vec)

    def test_017_t(self):
        self.slice = pdu_utils.pdu_slice('[0::-1]')
        self.connectUp()
        i_vec = pmt.init_u8vector(10, [0, 1, 2, 3, 4, 5, 6, 7, 8, 9])
        e_vec = pmt.init_u8vector(1, [0])
        self.runTest(i_vec, e_vec)

    def test_018_t(self):
        self.slice = pdu_utils.pdu_slice('[:0:1]')
        self.connectUp()
        i_vec = pmt.init_u8vector(10, [0, 1, 2, 3, 4, 5, 6, 7, 8, 9])
        e_vec = pmt.init_u8vector(0, [])
        self.runTest(i_vec, e_vec)

    def test_019_t(self):
        self.slice = pdu_utils.pdu_slice('[20:-20:-2]')
        self.connectUp()
        i_vec = pmt.init_u8vector(10, [0, 1, 2, 3, 4, 5, 6, 7, 8, 9])
        e_vec = pmt.init_u8vector(5, [9, 7, 5, 3, 1])
        self.runTest(i_vec, e_vec)

    def test_020_t(self):
        self.slice = pdu_utils.pdu_slice('[:20:4]')
        self.connectUp()
        i_vec = pmt.init_u8vector(10, [0, 1, 2, 3, 4, 5, 6, 7, 8, 9])
        e_vec = pmt.init_u8vector(3, [0, 4, 8])
        self.runTest(i_vec, e_vec)

    # Float and complex data type tests
    def test_021_t(self):
        self.slice = pdu_utils.pdu_slice('[:]')
        self.connectUp()
        i_vec = pmt.init_f32vector(10, [0.0, 1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9])
        e_vec = pmt.init_f32vector(10, [0.0, 1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9])
        self.runTest(i_vec, e_vec)

    def test_022_t(self):
        self.slice = pdu_utils.pdu_slice('[:]')
        self.connectUp()
        i_vec = pmt.init_c32vector(10, [0.+0.j, 1.+1.j, 2.+2.j, 3.+3.j, 4.+4.j, 5.+5.j, 6.+6.j, 7.+7.j, 8.+8.j, 9.+9.j])
        e_vec = pmt.init_c32vector(10, [0.+0.j, 1.+1.j, 2.+2.j, 3.+3.j, 4.+4.j, 5.+5.j, 6.+6.j, 7.+7.j, 8.+8.j, 9.+9.j])
        self.runTest(i_vec, e_vec)

    # Negative tests for slice input
    def test_023_t(self):
        self.slice = pdu_utils.pdu_slice('[:]')
        self.connectUp()
        self.runNegativeTest('')
        self.runNegativeTest(':')
        self.runNegativeTest('[]')
        self.runNegativeTest('[:')
        self.runNegativeTest(':]')
        self.runNegativeTest('[]:')
        self.runNegativeTest('][:]')
        self.runNegativeTest('[:::]')
        self.runNegativeTest('"[:]"')
        self.runNegativeTest('[a:2:3]')
        self.runNegativeTest('[start:]')
        self.runNegativeTest('[1:two:3]')
        self.runNegativeTest('[::[]')
        self.runNegativeTest('[--1:]')
        self.runNegativeTest('[:f9:]')
        self.runNegativeTest('[')
        self.runNegativeTest(']')
        self.runNegativeTest('[::0]')

   # Negative PDU tests
    def test_024_t(self):
        self.slice = pdu_utils.pdu_slice('[:]')
        self.connectUp()
        i_vec = pmt.init_u8vector(10, [0, 1, 2, 3, 4, 5, 6, 7, 8, 9])
        e_vec = pmt.init_u8vector(10, [0, 1, 2, 3, 4, 5, 6, 7, 8, 9])
        in_pdu = pmt.cons(pmt.make_dict(), i_vec)
        e_pdu = pmt.cons(pmt.make_dict(), e_vec)

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(pmt.intern("MALFORMED PDU"))
        time.sleep(.001)
        self.emitter.emit(pmt.cons(pmt.intern("NON-PDU"), pmt.intern("PAIR")))
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        print("test: ")
        print("data expected: " + repr(pmt.u8vector_elements(pmt.cdr(e_pdu))))
        print("data received: " + repr(pmt.u8vector_elements(pmt.cdr(self.debug.get_message(0)))))
        print

        self.assertTrue(pmt.equal(self.debug.get_message(0), e_pdu))

if __name__ == '__main__':
    gr_unittest.run(qa_pdu_slice)
