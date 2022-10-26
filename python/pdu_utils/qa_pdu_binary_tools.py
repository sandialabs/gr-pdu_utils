#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright 2018-2021 National Technology & Engineering Solutions of Sandia, LLC
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


class qa_pdu_binary_tools (gr_unittest.TestCase):

    def setUp(self):
        self.tb = gr.top_block()
        self.emitter = pdu_utils.message_emitter()
        self.debug = blocks.message_debug()

    def connectUp(self):
        self.tb.msg_connect((self.emitter, 'msg'), (self.dut, 'pdu_in'))
        self.tb.msg_connect((self.dut, 'pdu_out'), (self.debug, 'store'))

    def tearDown(self):
        self.tb = None

    def test_bit_flip(self):
        self.dut = pdu_utils.pdu_binary_tools(0)
        self.connectUp()

        i_vec = pmt.init_u8vector(6, [1, 0, 0, 1, 0, 1])
        e_vec = pmt.init_u8vector(6, [0, 1, 1, 0, 1, 0])
        in_pdu = pmt.cons(pmt.make_dict(), i_vec)
        e_pdu = pmt.cons(pmt.make_dict(), e_vec)

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        print("test bit_flip:")
        print("pdu expected: " + repr(pmt.car(e_pdu)))
        print("pdu got:      " + repr(pmt.car(self.debug.get_message(0))))
        print("data expected: " + repr(pmt.u8vector_elements(pmt.cdr(e_pdu))))
        print("data got:      " + repr(pmt.u8vector_elements(pmt.cdr(self.debug.get_message(0)))))
        print(self.assertTrue(pmt.equal(self.debug.get_message(0), e_pdu)))

    def test_to_pam(self):
        self.dut = pdu_utils.pdu_binary_tools(1) #TO_NRZ
        self.connectUp()

        i_vec = pmt.init_u8vector(6, [1, 0, 0, 1, 0, 1])
        e_vec = pmt.init_f32vector(6, [1, -1, -1, 1, -1, 1])
        in_pdu = pmt.cons(pmt.make_dict(), i_vec)
        e_pdu = pmt.cons(pmt.make_dict(), e_vec)

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        print("test to_pam:")
        print("pdu expected: " + repr(pmt.car(e_pdu)))
        print("pdu got:      " + repr(pmt.car(self.debug.get_message(0))))
        print("data expected: " + repr(pmt.f32vector_elements(pmt.cdr(e_pdu))))
        print("data got:      " + repr(pmt.f32vector_elements(pmt.cdr(self.debug.get_message(0)))))

        self.assertTrue(pmt.equal(self.debug.get_message(0), e_pdu))

    def test_from_pam(self):
        self.dut = pdu_utils.pdu_binary_tools(2) #FROM_NRZ
        self.connectUp()

        i_vec = pmt.init_f32vector(6, [1, -1, -1, 1, -1, 1])
        e_vec = pmt.init_u8vector(6, [1, 0, 0, 1, 0, 1])
        in_pdu = pmt.cons(pmt.make_dict(), i_vec)
        e_pdu = pmt.cons(pmt.make_dict(), e_vec)

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        print("test from_pam:")
        print("pdu expected: " + repr(pmt.car(e_pdu)))
        print("pdu got:      " + repr(pmt.car(self.debug.get_message(0))))
        print("data expected: " + repr(pmt.u8vector_elements(pmt.cdr(e_pdu))))
        print("data got:      " + repr(pmt.u8vector_elements(pmt.cdr(self.debug.get_message(0)))))

        self.assertTrue(pmt.equal(self.debug.get_message(0), e_pdu))

    def test_slice(self):
        self.dut = pdu_utils.pdu_binary_tools(3) #SLICE
        self.connectUp()

        i_vec = pmt.init_f32vector(6, [2, -1, -2, .1, -.1, 1000])
        e_vec = pmt.init_u8vector(6, [1, 0, 0, 1, 0, 1])
        in_pdu = pmt.cons(pmt.make_dict(), i_vec)
        e_pdu = pmt.cons(pmt.make_dict(), e_vec)

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        print("test slice:")
        print("pdu expected: " + repr(pmt.car(e_pdu)))
        print("pdu got:      " + repr(pmt.car(self.debug.get_message(0))))
        print("data expected: " + repr(pmt.u8vector_elements(pmt.cdr(e_pdu))))
        print("data got:      " + repr(pmt.u8vector_elements(pmt.cdr(self.debug.get_message(0)))))

        self.assertTrue(pmt.equal(self.debug.get_message(0), e_pdu))

    def test_endian(self):
        self.dut = pdu_utils.pdu_binary_tools(4) #ENDIAN_SWAP8
        self.connectUp()

        i_vec = pmt.init_u8vector(16, [1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0])
        e_vec = pmt.init_u8vector(16, [1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1])
        in_pdu = pmt.cons(pmt.make_dict(), i_vec)
        e_pdu = pmt.cons(pmt.make_dict(), e_vec)

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        print("test endian_swap8:")
        print("pdu expected: " + repr(pmt.car(e_pdu)))
        print("pdu got:      " + repr(pmt.car(self.debug.get_message(0))))
        print("data expected: " + repr(pmt.u8vector_elements(pmt.cdr(e_pdu))))
        print("data got:      " + repr(pmt.u8vector_elements(pmt.cdr(self.debug.get_message(0)))))

        self.assertTrue(pmt.equal(self.debug.get_message(0), e_pdu))

    def test_manchester_encode(self):
        self.dut = pdu_utils.pdu_binary_tools(5) #MANCHESTER_ENCODE
        self.connectUp()

        i_vec = pmt.init_u8vector(8, [1, 1, 0, 0, 1, 0, 1, 0])
        e_vec = pmt.init_u8vector(16, [1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1])
        in_pdu = pmt.cons(pmt.make_dict(), i_vec)
        e_pdu = pmt.cons(pmt.make_dict(), e_vec)

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        print("test manchester encode:")
        print("pdu expected: " + repr(pmt.car(e_pdu)))
        print("pdu got:      " + repr(pmt.car(self.debug.get_message(0))))
        print("data expected: " + repr(pmt.u8vector_elements(pmt.cdr(e_pdu))))
        print("data got:      " + repr(pmt.u8vector_elements(pmt.cdr(self.debug.get_message(0)))))

        self.assertTrue(pmt.equal(self.debug.get_message(0), e_pdu))

    def test_manchester_encode_nrz(self):
        self.dut = pdu_utils.pdu_binary_tools(5) #MANCHESTER_ENCODE
        self.connectUp()

        i_vec = pmt.init_f32vector(8, [1, 1, -1, -1, 1, -1, 1, -1])
        e_vec = pmt.init_f32vector(16, [1, -1, 1, -1, -1, 1, -1, 1, 1, -1, -1, 1, 1, -1, -1, 1])
        in_pdu = pmt.cons(pmt.make_dict(), i_vec)
        e_pdu = pmt.cons(pmt.make_dict(), e_vec)

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        print("test manchester encode nrz:")
        print("pdu expected: " + repr(pmt.car(e_pdu)))
        print("pdu got:      " + repr(pmt.car(self.debug.get_message(0))))
        print("data expected: " + repr(pmt.f32vector_elements(pmt.cdr(e_pdu))))
        print("data got:      " + repr(pmt.f32vector_elements(pmt.cdr(self.debug.get_message(0)))))

        self.assertTrue(pmt.equal(self.debug.get_message(0), e_pdu))

    def test_manchester_decode(self):
        self.dut = pdu_utils.pdu_binary_tools(6) #MANCHESTER_DECODE
        self.connectUp()

        i_vec = pmt.init_u8vector(16, [1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1])
        e_vec = pmt.init_u8vector(8, [1, 1, 0, 0, 1, 0, 1, 0])
        in_pdu = pmt.cons(pmt.make_dict(), i_vec)
        e_pdu = pmt.cons(pmt.make_dict(), e_vec)

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        print("test manchester decode:")
        print("pdu expected: " + repr(pmt.car(e_pdu)))
        print("pdu got:      " + repr(pmt.car(self.debug.get_message(0))))
        print("data expected: " + repr(pmt.u8vector_elements(pmt.cdr(e_pdu))))
        print("data got:      " + repr(pmt.u8vector_elements(pmt.cdr(self.debug.get_message(0)))))

        self.assertTrue(pmt.equal(self.debug.get_message(0), e_pdu))

    def test_manchester_decode_nrz(self):
        self.dut = pdu_utils.pdu_binary_tools(6) #MANCHESTER_DECODE
        self.connectUp()

        i_vec = pmt.init_f32vector(16, [1, -1, 1, -1, -1, 1, -1, 1, 1, -1, -1, 1, 1, -1, -1, 1])
        e_vec = pmt.init_f32vector(8, [1, 1, -1, -1, 1, -1, 1, -1])
        in_pdu = pmt.cons(pmt.make_dict(), i_vec)
        e_pdu = pmt.cons(pmt.make_dict(), e_vec)

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        print("test manchester decode nrz:")
        print("pdu expected: " + repr(pmt.car(e_pdu)))
        print("pdu got:      " + repr(pmt.car(self.debug.get_message(0))))
        print("data expected: " + repr(pmt.f32vector_elements(pmt.cdr(e_pdu))))
        print("data got:      " + repr(pmt.f32vector_elements(pmt.cdr(self.debug.get_message(0)))))

        self.assertTrue(pmt.equal(self.debug.get_message(0), e_pdu))


if __name__ == '__main__':
    gr_unittest.run(qa_pdu_binary_tools)
