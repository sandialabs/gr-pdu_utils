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
import pdu_utils_swig as pdu_utils
import pmt
import time


class qa_pdu_fir_filter (gr_unittest.TestCase):

    def setUp(self):
        self.tb = gr.top_block()
        self.emitter = pdu_utils.message_emitter()
        self.debug = blocks.message_debug()

    def connectUp(self):
        self.tb.msg_connect((self.emitter, 'msg'), (self.dut, 'pdu_in'))
        self.tb.msg_connect((self.dut, 'pdu_out'), (self.debug, 'store'))

    def tearDown(self):
        self.tb = None

    def test_001_uint8(self):
        '''
        uint8_t input data, no decimation, no filter
        '''
        self.dut = pdu_utils.pdu_fir_filter(1, [1.0])
        self.connectUp()

        i_data = [1, 0, 0, 0] * 5
        i_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("sample_rate"), pmt.from_double(1000.0))
        in_pdu = pmt.cons(i_meta, pmt.init_u8vector(len(i_data), i_data))

        e_data = [1, 0, 0, 0] * 5
        e_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("sample_rate"), pmt.from_double(1000.0))
        e_pdu = pmt.cons(e_meta, pmt.init_u8vector(len(e_data), e_data))

        self.tb.start()
        time.sleep(.01)
        self.emitter.emit(in_pdu)
        time.sleep(.1)
        self.tb.stop()
        self.tb.wait()

        #print("test_001:")
        #print("pdu expected: " + repr(pmt.car(e_pdu)))
        #print("pdu got:      " + repr(pmt.car(self.debug.get_message(0))))
        #print("data expected: " + repr(pmt.to_python(pmt.cdr(e_pdu))))
        #print("data got:      " + repr(pmt.to_python(pmt.cdr(self.debug.get_message(0)))))
        #print

        self.assertTrue(pmt.equal(self.debug.get_message(0), e_pdu))

    def test_001_float(self):
        '''
        uint8_t input data, no decimation, no filter
        '''
        self.dut = pdu_utils.pdu_fir_filter(1, [1.0])
        self.connectUp()

        i_data = [1, 0, 0, 0] * 5
        i_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("sample_rate"), pmt.from_double(1000.0))
        in_pdu = pmt.cons(i_meta, pmt.init_f32vector(len(i_data), i_data))

        e_data = [1, 0, 0, 0] * 5
        e_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("sample_rate"), pmt.from_double(1000.0))
        e_pdu = pmt.cons(e_meta, pmt.init_f32vector(len(e_data), e_data))

        self.tb.start()
        time.sleep(.01)
        self.emitter.emit(in_pdu)
        time.sleep(.1)
        self.tb.stop()
        self.tb.wait()

        #print("test_001:")
        #print("pdu expected: " + repr(pmt.car(e_pdu)))
        #print("pdu got:      " + repr(pmt.car(self.debug.get_message(0))))
        #print("data expected: " + repr(pmt.to_python(pmt.cdr(e_pdu))))
        #print("data got:      " + repr(pmt.to_python(pmt.cdr(self.debug.get_message(0)))))
        #print

        self.assertTrue(pmt.equal(self.debug.get_message(0), e_pdu))

    def test_001_c32(self):
        '''
        uint8_t input data, no decimation, no filter
        '''
        self.dut = pdu_utils.pdu_fir_filter(1, [1.0])
        self.connectUp()

        i_data = [1, 0, 0, 0] * 5
        i_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("sample_rate"), pmt.from_double(1000.0))
        in_pdu = pmt.cons(i_meta, pmt.init_c32vector(len(i_data), i_data))

        e_data = [1, 0, 0, 0] * 5
        e_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("sample_rate"), pmt.from_double(1000.0))
        e_pdu = pmt.cons(e_meta, pmt.init_c32vector(len(e_data), e_data))

        self.tb.start()
        time.sleep(.01)
        self.emitter.emit(in_pdu)
        time.sleep(.1)
        self.tb.stop()
        self.tb.wait()

        #print("test_001:")
        #print("pdu expected: " + repr(pmt.car(e_pdu)))
        #print("pdu got:      " + repr(pmt.car(self.debug.get_message(0))))
        #print("data expected: " + repr(pmt.to_python(pmt.cdr(e_pdu))))
        #print("data got:      " + repr(pmt.to_python(pmt.cdr(self.debug.get_message(0)))))
        #print

        self.assertTrue(pmt.equal(self.debug.get_message(0), e_pdu))

    def test_002_uint8(self):
        '''
        uint8_t input data, no decimation, attenuation filter
        '''
        self.dut = pdu_utils.pdu_fir_filter(1, [0.5])
        self.connectUp()

        i_data = [2, 0, 0, 0] * 5
        i_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("sample_rate"), pmt.from_double(1000.0))
        in_pdu = pmt.cons(i_meta, pmt.init_u8vector(len(i_data), i_data))

        e_data = [1, 0, 0, 0] * 5
        e_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("sample_rate"), pmt.from_double(1000.0))
        e_pdu = pmt.cons(e_meta, pmt.init_u8vector(len(e_data), e_data))

        self.tb.start()
        time.sleep(.01)
        self.emitter.emit(in_pdu)
        time.sleep(.1)
        self.tb.stop()
        self.tb.wait()

        #print("test_002:")
        #print("pdu expected: " + repr(pmt.car(e_pdu)))
        #print("pdu got:      " + repr(pmt.car(self.debug.get_message(0))))
        #print("data expected: " + repr(pmt.to_python(pmt.cdr(e_pdu))))
        #print("data got:      " + repr(pmt.to_python(pmt.cdr(self.debug.get_message(0)))))
        #print

        self.assertTrue(pmt.equal(self.debug.get_message(0), e_pdu))

    def test_002_float(self):
        '''
        uint8_t input data, no decimation, attenuation filter
        '''
        self.dut = pdu_utils.pdu_fir_filter(1, [0.5])
        self.connectUp()

        i_data = [2, 0, 0, 0] * 5
        i_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("sample_rate"), pmt.from_double(1000.0))
        in_pdu = pmt.cons(i_meta, pmt.init_f32vector(len(i_data), i_data))

        e_data = [1, 0, 0, 0] * 5
        e_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("sample_rate"), pmt.from_double(1000.0))
        e_pdu = pmt.cons(e_meta, pmt.init_f32vector(len(e_data), e_data))

        self.tb.start()
        time.sleep(.01)
        self.emitter.emit(in_pdu)
        time.sleep(.1)
        self.tb.stop()
        self.tb.wait()

        #print("test_002:")
        #print("pdu expected: " + repr(pmt.car(e_pdu)))
        #print("pdu got:      " + repr(pmt.car(self.debug.get_message(0))))
        #print("data expected: " + repr(pmt.to_python(pmt.cdr(e_pdu))))
        #print("data got:      " + repr(pmt.to_python(pmt.cdr(self.debug.get_message(0)))))
        #print

        self.assertTrue(pmt.equal(self.debug.get_message(0), e_pdu))

    def test_002_c32(self):
        '''
        uint8_t input data, no decimation, attenuation filter
        '''
        self.dut = pdu_utils.pdu_fir_filter(1, [0.5])
        self.connectUp()

        i_data = [2, 0, 0, 0] * 5
        i_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("sample_rate"), pmt.from_double(1000.0))
        in_pdu = pmt.cons(i_meta, pmt.init_c32vector(len(i_data), i_data))

        e_data = [1, 0, 0, 0] * 5
        e_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("sample_rate"), pmt.from_double(1000.0))
        e_pdu = pmt.cons(e_meta, pmt.init_c32vector(len(e_data), e_data))

        self.tb.start()
        time.sleep(.01)
        self.emitter.emit(in_pdu)
        time.sleep(.1)
        self.tb.stop()
        self.tb.wait()

        #print("test_002:")
        #print("pdu expected: " + repr(pmt.car(e_pdu)))
        #print("pdu got:      " + repr(pmt.car(self.debug.get_message(0))))
        #print("data expected: " + repr(pmt.to_python(pmt.cdr(e_pdu))))
        #print("data got:      " + repr(pmt.to_python(pmt.cdr(self.debug.get_message(0)))))
        #print

        self.assertTrue(pmt.equal(self.debug.get_message(0), e_pdu))

    def test_003_uint8(self):
        '''
        uint8_t input data, decimation, no filter
        '''
        self.dut = pdu_utils.pdu_fir_filter(2, [1.0])
        self.connectUp()

        i_data = [1, 0, 0, 0] * 5
        i_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("sample_rate"), pmt.from_double(1000.0))
        in_pdu = pmt.cons(i_meta, pmt.init_u8vector(len(i_data), i_data))

        e_data = [1, 0] * 5
        e_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("sample_rate"), pmt.from_double(500.0))
        e_pdu = pmt.cons(e_meta, pmt.init_u8vector(len(e_data), e_data))

        self.tb.start()
        time.sleep(.01)
        self.emitter.emit(in_pdu)
        time.sleep(.1)
        self.tb.stop()
        self.tb.wait()

        #print("test_003:")
        #print("pdu expected: " + repr(pmt.car(e_pdu)))
        #print("pdu got:      " + repr(pmt.car(self.debug.get_message(0))))
        #print("data expected: " + repr(pmt.to_python(pmt.cdr(e_pdu))))
        #print("data got:      " + repr(pmt.to_python(pmt.cdr(self.debug.get_message(0)))))
        #print

        self.assertTrue(pmt.equal(self.debug.get_message(0), e_pdu))

    def test_003_float(self):
        '''
        uint8_t input data, decimation, no filter
        '''
        self.dut = pdu_utils.pdu_fir_filter(2, [1.0])
        self.connectUp()

        i_data = [1, 0, 0, 0] * 5
        i_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("sample_rate"), pmt.from_double(1000.0))
        in_pdu = pmt.cons(i_meta, pmt.init_f32vector(len(i_data), i_data))

        e_data = [1, 0] * 5
        e_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("sample_rate"), pmt.from_double(500.0))
        e_pdu = pmt.cons(e_meta, pmt.init_f32vector(len(e_data), e_data))

        self.tb.start()
        time.sleep(.01)
        self.emitter.emit(in_pdu)
        time.sleep(.1)
        self.tb.stop()
        self.tb.wait()

        #print("test_003:")
        #print("pdu expected: " + repr(pmt.car(e_pdu)))
        #print("pdu got:      " + repr(pmt.car(self.debug.get_message(0))))
        #print("data expected: " + repr(pmt.to_python(pmt.cdr(e_pdu))))
        #print("data got:      " + repr(pmt.to_python(pmt.cdr(self.debug.get_message(0)))))
        #print

        self.assertTrue(pmt.equal(self.debug.get_message(0), e_pdu))

    def test_003_c32(self):
        '''
        uint8_t input data, decimation, no filter
        '''
        self.dut = pdu_utils.pdu_fir_filter(2, [1.0])
        self.connectUp()

        i_data = [1, 0, 0, 0] * 5
        i_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("sample_rate"), pmt.from_double(1000.0))
        in_pdu = pmt.cons(i_meta, pmt.init_c32vector(len(i_data), i_data))

        e_data = [1, 0] * 5
        e_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("sample_rate"), pmt.from_double(500.0))
        e_pdu = pmt.cons(e_meta, pmt.init_c32vector(len(e_data), e_data))

        self.tb.start()
        time.sleep(.01)
        self.emitter.emit(in_pdu)
        time.sleep(.1)
        self.tb.stop()
        self.tb.wait()

        #print("test_003:")
        #print("pdu expected: " + repr(pmt.car(e_pdu)))
        #print("pdu got:      " + repr(pmt.car(self.debug.get_message(0))))
        #print("data expected: " + repr(pmt.to_python(pmt.cdr(e_pdu))))
        #print("data got:      " + repr(pmt.to_python(pmt.cdr(self.debug.get_message(0)))))
        #print

        self.assertTrue(pmt.equal(self.debug.get_message(0), e_pdu))

    def test_004_f32(self):
        '''
        float input data, no decimation, odd number of taps
        '''
        self.dut = pdu_utils.pdu_fir_filter(1, [.2]*5)
        self.connectUp()

        i_data = [1,] * 10
        i_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("sample_rate"), pmt.from_double(1000.0))
        in_pdu = pmt.cons(i_meta, pmt.init_f32vector(len(i_data), i_data))

        e_data = [.6, .8, ] + [1,] * 6 + [.8, .6]
        e_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("sample_rate"), pmt.from_double(1000.0))
        e_pdu = pmt.cons(e_meta, pmt.init_f32vector(len(e_data), e_data))

        self.tb.start()
        time.sleep(.01)
        self.emitter.emit(in_pdu)
        time.sleep(.1)
        self.tb.stop()
        self.tb.wait()

        #print("test_004:")
        #print("pdu expected: " + repr(pmt.car(e_pdu)))
        #print("pdu got:      " + repr(pmt.car(self.debug.get_message(0))))
        #print("data expected: " + repr(pmt.to_python(pmt.cdr(e_pdu))))
        #print("data got:      " + repr(pmt.to_python(pmt.cdr(self.debug.get_message(0)))))
        #print

        self.assertTrue(pmt.equal(self.debug.get_message(0), e_pdu))

    def test_004_c32(self):
        '''
        complex input data, no decimation, odd number of taps
        '''
        self.dut = pdu_utils.pdu_fir_filter(1, [.2]*5)
        self.connectUp()

        i_data = [1,] * 10
        i_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("sample_rate"), pmt.from_double(1000.0))
        in_pdu = pmt.cons(i_meta, pmt.init_c32vector(len(i_data), i_data))

        e_data = [.6, .8, ] + [1,] * 6 + [.8, .6]
        e_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("sample_rate"), pmt.from_double(1000.0))
        e_pdu = pmt.cons(e_meta, pmt.init_c32vector(len(e_data), e_data))

        self.tb.start()
        time.sleep(.01)
        self.emitter.emit(in_pdu)
        time.sleep(.1)
        self.tb.stop()
        self.tb.wait()

        #print("test_004:")
        #print("pdu expected: " + repr(pmt.car(e_pdu)))
        #print("pdu got:      " + repr(pmt.car(self.debug.get_message(0))))
        #print("data expected: " + repr(pmt.to_python(pmt.cdr(e_pdu))))
        #print("data got:      " + repr(pmt.to_python(pmt.cdr(self.debug.get_message(0)))))
        #print

        self.assertTrue(pmt.equal(self.debug.get_message(0), e_pdu))

    def test_004_u8(self):
        '''
        u8 input data, no decimation, odd number of taps
        '''
        self.dut = pdu_utils.pdu_fir_filter(1, [2]*5)
        self.connectUp()

        i_data = [1,] * 10
        i_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("sample_rate"), pmt.from_double(1000.0))
        in_pdu = pmt.cons(i_meta, pmt.init_u8vector(len(i_data), i_data))

        e_data = [6, 8, ] + [10,] * 6 + [8, 6]
        e_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("sample_rate"), pmt.from_double(1000.0))
        e_pdu = pmt.cons(e_meta, pmt.init_u8vector(len(e_data), e_data))

        self.tb.start()
        time.sleep(.01)
        self.emitter.emit(in_pdu)
        time.sleep(.1)
        self.tb.stop()
        self.tb.wait()

        #print("test_004:")
        #print("pdu expected: " + repr(pmt.car(e_pdu)))
        #print("pdu got:      " + repr(pmt.car(self.debug.get_message(0))))
        #print("data expected: " + repr(pmt.to_python(pmt.cdr(e_pdu))))
        #print("data got:      " + repr(pmt.to_python(pmt.cdr(self.debug.get_message(0)))))
        #print

        self.assertTrue(pmt.equal(self.debug.get_message(0), e_pdu))

    def test_005_f32(self):
        '''
        float input data, no decimation, even number of taps
        '''
        self.dut = pdu_utils.pdu_fir_filter(1, [.2]*6)
        self.connectUp()

        i_data = [1,] * 10
        i_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("sample_rate"), pmt.from_double(100.0))
        i_meta = pmt.dict_add(i_meta, pmt.intern("start_time"), pmt.from_double(9.9))
        in_pdu = pmt.cons(i_meta, pmt.init_f32vector(len(i_data), i_data))

        e_data = [.6, .8, 1, ] + [1.2,] * 5 + [1, .8, .6]
        e_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("sample_rate"), pmt.from_double(100.0))
        e_meta = pmt.dict_add(e_meta, pmt.intern("start_time"), pmt.from_double(9.895))
        e_pdu = pmt.cons(e_meta, pmt.init_f32vector(len(e_data), e_data))

        self.tb.start()
        time.sleep(.01)
        self.emitter.emit(in_pdu)
        time.sleep(.1)
        self.tb.stop()
        self.tb.wait()

        #print("test_005:")
        #print("pdu expected: " + repr(pmt.car(e_pdu)))
        #print("pdu got:      " + repr(pmt.car(self.debug.get_message(0))))
        #print("data expected: " + repr(pmt.to_python(pmt.cdr(e_pdu))))
        #print("data got:      " + repr(pmt.to_python(pmt.cdr(self.debug.get_message(0)))))
        #print

        self.assertTrue(pmt.equal(self.debug.get_message(0), e_pdu))

    def test_005_c32(self):
        '''
        complex input data, no decimation, even number of taps
        '''
        self.dut = pdu_utils.pdu_fir_filter(1, [.2]*6)
        self.connectUp()

        i_data = [1,] * 10
        i_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("sample_rate"), pmt.from_double(100.0))
        i_meta = pmt.dict_add(i_meta, pmt.intern("start_time"), pmt.from_double(9.9))
        in_pdu = pmt.cons(i_meta, pmt.init_c32vector(len(i_data), i_data))

        e_data = [.6, .8, 1, ] + [1.2,] * 5 + [1, .8, .6]
        e_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("sample_rate"), pmt.from_double(100.0))
        e_meta = pmt.dict_add(e_meta, pmt.intern("start_time"), pmt.from_double(9.895))
        e_pdu = pmt.cons(e_meta, pmt.init_c32vector(len(e_data), e_data))

        self.tb.start()
        time.sleep(.01)
        self.emitter.emit(in_pdu)
        time.sleep(.1)
        self.tb.stop()
        self.tb.wait()

        #print("test_005:")
        #print("pdu expected: " + repr(pmt.car(e_pdu)))
        #print("pdu got:      " + repr(pmt.car(self.debug.get_message(0))))
        #print("data expected: " + repr(pmt.to_python(pmt.cdr(e_pdu))))
        #print("data got:      " + repr(pmt.to_python(pmt.cdr(self.debug.get_message(0)))))
        #print

        self.assertTrue(pmt.equal(self.debug.get_message(0), e_pdu))

    def test_005_u8(self):
        '''
        u8 input data, no decimation, even number of taps
        '''
        self.dut = pdu_utils.pdu_fir_filter(1, [1]*4)
        self.connectUp()

        i_data = [1,] * 10
        i_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("sample_rate"), pmt.from_double(100.0))
        i_meta = pmt.dict_add(i_meta, pmt.intern("start_time"), pmt.from_double(9.9))
        in_pdu = pmt.cons(i_meta, pmt.init_u8vector(len(i_data), i_data))

        e_data = [2, 3] + [4,] * 7 + [3, 2]
        e_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("sample_rate"), pmt.from_double(100.0))
        e_meta = pmt.dict_add(e_meta, pmt.intern("start_time"), pmt.from_double(9.895))
        e_pdu = pmt.cons(e_meta, pmt.init_u8vector(len(e_data), e_data))

        self.tb.start()
        time.sleep(.01)
        self.emitter.emit(in_pdu)
        time.sleep(.1)
        self.tb.stop()
        self.tb.wait()

        #print("test_005:")
        #print("pdu expected: " + repr(pmt.car(e_pdu)))
        #print("pdu got:      " + repr(pmt.car(self.debug.get_message(0))))
        #print("data expected: " + repr(pmt.to_python(pmt.cdr(e_pdu))))
        #print("data got:      " + repr(pmt.to_python(pmt.cdr(self.debug.get_message(0)))))
        #print

        self.assertTrue(pmt.equal(self.debug.get_message(0), e_pdu))

    def test_005_f32(self):
        '''
        float input data, complicated input, decimation, and filtering
        '''
        taps=[-0.1, -0.2, -0.3,  0. ,  0.8,  1.4,  0.7, -1.9, -4.5, -3.8,  2.9,
              14.4, 25.5, 30.1, 30.1, 25.5, 14.4,  2.9, -3.8, -4.5, -1.9,  0.7,
               1.4,  0.8,  0. , -0.3, -0.2, -0.1]
        print(len(taps))
        self.dut = pdu_utils.pdu_fir_filter(4, taps)
        self.connectUp()

        i_data=[ 0.  ,  0.25,  0.48,  0.68,  0.84,  0.95,  1.  ,  0.98,  0.91,
                 0.78,  0.6 ,  0.38,  0.14, -0.11, -0.35, -0.57, -0.76, -0.89,
                -0.98, -1.  , -0.96, -0.86, -0.71, -0.51, -0.28, -0.03,  0.22,
                 0.45,  0.66,  0.82,  0.94,  0.99,  0.99,  0.92,  0.8 ,  0.62,
                 0.41,  0.17, -0.08, -0.32, -0.54, -0.73, -0.88, -0.97, -1.  ,
                -0.97, -0.88, -0.73, -0.54, -0.31, -0.07,  0.18,  0.42,  0.63,
                 0.8 ,  0.93,  0.99,  0.99,  0.93,  0.82,  0.65,  0.44,  0.21,
                -0.04, -0.29, -0.52, -0.71, -0.86, -0.96, -1.  , -0.98, -0.89]
        i_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("sample_rate"), pmt.from_double(40e3))
        i_meta = pmt.dict_add(i_meta, pmt.intern("start_time"), pmt.from_double(65.4321))
        in_pdu = pmt.cons(i_meta, pmt.init_f32vector(len(i_data), i_data))

        e_data = [   8.344998,   95.737   ,  121.949005,   33.454994,  -85.351006,
                  -126.05898 ,  -50.684998,   71.381004,  127.386   ,   66.26399 ,
                   -55.451004, -126.658005,  -81.652   ,   38.550003,  123.046005,
                    94.08399 ,  -19.649996, -123.254005]
        e_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("sample_rate"), pmt.from_double(10e3))
        e_meta = pmt.dict_add(e_meta, pmt.intern("start_time"), pmt.from_double(65.432050))
        e_pdu = pmt.cons(e_meta, pmt.init_f32vector(len(e_data), e_data))

        self.tb.start()
        time.sleep(.01)
        self.emitter.emit(in_pdu)
        time.sleep(.1)
        self.tb.stop()
        self.tb.wait()

        #print("test_005:")
        #print("pdu expected: " + repr(pmt.car(e_pdu)))
        #print("pdu got:      " + repr(pmt.car(self.debug.get_message(0))))
        #print("data expected: " + repr(pmt.to_python(pmt.cdr(e_pdu))))
        #print("data got:      " + repr(pmt.to_python(pmt.cdr(self.debug.get_message(0)))))
        #print

        self.assertTrue(pmt.equal(self.debug.get_message(0), e_pdu))

    def test_006_c32(self):
        '''
        complex input data, complicated input, decimation, and filtering
        '''
        self.dut = pdu_utils.pdu_fir_filter(2, [2.5, 5, 10, 20, 10, 5, 2.5])
        self.connectUp()

        i_data = [1j, 1, -1j, -1, ]*4
        i_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("sample_rate"), pmt.from_double(22.2))
        i_meta = pmt.dict_add(i_meta, pmt.intern("start_time"), pmt.from_double(0))
        in_pdu = pmt.cons(i_meta, pmt.init_c32vector(len(i_data), i_data))

        e_data = [7.5+15.j, 2.5-10.j, 10.j, -0-10.j, +10.j, -0-10.j, +10.j, -2.5-15.j]
        e_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("start_time"), pmt.from_double(0))
        e_meta = pmt.dict_add(e_meta, pmt.intern("sample_rate"), pmt.from_double(11.1))
        e_pdu = pmt.cons(e_meta, pmt.init_c32vector(len(e_data), e_data))

        self.tb.start()
        time.sleep(.01)
        self.emitter.emit(in_pdu)
        time.sleep(.1)
        self.tb.stop()
        self.tb.wait()

        #print("test_005:")
        #print("pdu expected: " + repr(pmt.car(e_pdu)))
        #print("pdu got:      " + repr(pmt.car(self.debug.get_message(0))))
        #print("data expected: " + repr(pmt.to_python(pmt.cdr(e_pdu))))
        #print("data got:      " + repr(pmt.to_python(pmt.cdr(self.debug.get_message(0)))))
        #print

        self.assertTrue(pmt.equal(self.debug.get_message(0), e_pdu))

    def test_006_u8(self):
        '''
        u8 input data, complicated input, decimation, and filtering
        '''
        self.dut = pdu_utils.pdu_fir_filter(3, [1,2,4,3]*4)
        self.connectUp()

        i_data = [4, 4, 4, 0, 0, 0] * 10
        i_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("sample_rate"), pmt.from_double(3.0))
        i_meta = pmt.dict_add(i_meta, pmt.intern("start_time"), pmt.from_double(33))
        in_pdu = pmt.cons(i_meta, pmt.init_u8vector(len(i_data), i_data))

        e_data = [48, 60, 72, 88, 72, 88, 72, 88, 72, 88, 72, 88, 72, 88, 72, 88, 72, 88, 60, 56]
        e_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("sample_rate"), pmt.from_double(1.0))
        e_meta = pmt.dict_add(e_meta, pmt.intern("start_time"), pmt.from_double(32.5))
        e_pdu = pmt.cons(e_meta, pmt.init_u8vector(len(e_data), e_data))

        self.tb.start()
        time.sleep(.01)
        self.emitter.emit(in_pdu)
        time.sleep(.1)
        self.tb.stop()
        self.tb.wait()

        #print("test_005:")
        #print("pdu expected: " + repr(pmt.car(e_pdu)))
        #print("pdu got:      " + repr(pmt.car(self.debug.get_message(0))))
        #print("data expected: " + repr(pmt.to_python(pmt.cdr(e_pdu))))
        #print("data got:      " + repr(pmt.to_python(pmt.cdr(self.debug.get_message(0)))))
        #print

        self.assertTrue(pmt.equal(self.debug.get_message(0), e_pdu))


if __name__ == '__main__':
    gr_unittest.run(qa_pdu_fir_filter)
