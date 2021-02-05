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
    import pdu_utils
except ImportError:
    import os
    import sys
    dirname, filename = os.path.split(os.path.abspath(__file__))
    sys.path.append(os.path.join(dirname, "bindings"))
    import pdu_utils
import pmt
import time


class qa_pdu_fine_time_measure (gr_unittest.TestCase):

    def setUp(self):
        self.tb = gr.top_block()
        self.emitter = pdu_utils.message_emitter()
        self.debug = blocks.message_debug()

    def connectUp(self):
        self.tb.msg_connect((self.emitter, 'msg'), (self.dut, 'pdu_in'))
        self.tb.msg_connect((self.dut, 'pdu_out'), (self.debug, 'store'))

    def tearDown(self):
        self.tb = None

    def test_float(self):
        '''
        Note: this is a negative test. Modules doesn't support this format, so this is testing that
        '''
        self.dut = pdu_utils.pdu_fine_time_measure(0.001, 0.002, 10, 0.1)
        self.connectUp()

        i_vec = pmt.init_f32vector(9, [0, 1, 2, 3.3, 4, 5, 6, 7, 8])
        in_pdu = pmt.cons(pmt.make_dict(), i_vec)

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        self.assertEqual(0, self.debug.num_messages())

    def test_uint8(self):
        '''
        Note: this is a negative test. Modules doesn't support this format, so this is testing that
        '''
        self.dut = pdu_utils.pdu_fine_time_measure(0.001, 0.002, 10, 0.1)
        self.connectUp()

        i_vec = pmt.init_u8vector(6, [0, 1, 2, 3, 4, 5])
        in_pdu = pmt.cons(pmt.make_dict(), i_vec)

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        self.assertEqual(0, self.debug.num_messages())

    def test_001(self):
        '''
        Input data perfectly matches params to pdu_fine_time_measure
        '''
        self.dut = pdu_utils.pdu_fine_time_measure(0.005, 0.005, 10, 15)
        self.connectUp()

        i_data = [0.1 + 0j] * 5 + [1.0 + 0j] * 990 + [0.1 + 0j] * 5
        i_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("start_time"), pmt.from_double(1.0))
        i_meta = pmt.dict_add(i_meta, pmt.intern("sample_rate"), pmt.from_float(1000.0))
        i_meta = pmt.dict_add(i_meta, pmt.intern("duration"), pmt.from_float(1000.0))
        in_pdu = pmt.cons(i_meta, pmt.init_c32vector(len(i_data), i_data))

        e_data = [1.0 + 0j] * 990 + [0.1 + 0j] * 5
        e_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("sample_rate"), pmt.from_float(1000.0))
        e_meta = pmt.dict_add(e_meta, pmt.intern("start_time"), pmt.from_double(1.005))
        e_meta = pmt.dict_add(e_meta, pmt.intern("duration"), pmt.from_float(999.995))
        e_pdu = pmt.cons(e_meta, pmt.init_c32vector(len(e_data), e_data))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        #print("test_001:")
        #print("pdu expected: " + repr(pmt.car(e_pdu)))
        #print("pdu got:      " + repr(pmt.car(self.debug.get_message(0))))
        #print("data expected: " + repr(pmt.to_python(pmt.cdr(e_pdu))))
        #print("data got:      " + repr(pmt.to_python(pmt.cdr(self.debug.get_message(0)))))
        #print

        self.assertTrue(pmt.equal(self.debug.get_message(0), e_pdu))

    def test_002(self):
        '''
        Input data has extra quiet time front and rear
        '''
        self.dut = pdu_utils.pdu_fine_time_measure(0.005, 0.005, 10, 15)
        self.connectUp()

        i_data = [0.1 + 0j] * 10 + [1.0 + 0j] * 980 + [0.1 + 0j] * 10
        i_vec = pmt.init_c32vector(3, [3 + 4j, 1 + 0j, 0 + 1j])
        i_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("start_time"), pmt.from_double(1.0))
        i_meta = pmt.dict_add(i_meta, pmt.intern("sample_rate"), pmt.from_float(1000.0))
        i_meta = pmt.dict_add(i_meta, pmt.intern("duration"), pmt.from_float(1000.0))
        in_pdu = pmt.cons(i_meta, pmt.init_c32vector(len(i_data), i_data))

        e_data = [1.0 + 0j] * 980 + [0.1 + 0j] * 10
        e_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("sample_rate"), pmt.from_float(1000.0))
        e_meta = pmt.dict_add(e_meta, pmt.intern("start_time"), pmt.from_double(1.01))
        e_meta = pmt.dict_add(e_meta, pmt.intern("duration"), pmt.from_float(999.99))
        e_pdu = pmt.cons(e_meta, pmt.init_c32vector(len(e_data), e_data))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        #print("test_002:")
        #print("pdu expected: " + repr(pmt.car(e_pdu)))
        #print("pdu got:      " + repr(pmt.car(self.debug.get_message(0))))
        #print("data expected: " + repr(pmt.to_python(pmt.cdr(e_pdu))))
        #print("data got:      " + repr(pmt.to_python(pmt.cdr(self.debug.get_message(0)))))
        #print

        self.assertTrue(pmt.equal(self.debug.get_message(0), e_pdu))


if __name__ == '__main__':
    gr_unittest.run(qa_pdu_fine_time_measure)
