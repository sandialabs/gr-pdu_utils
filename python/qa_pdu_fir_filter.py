#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright 2018 National Technology & Engineering Solutions of Sandia, LLC (NTESS).
# Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government retains
# certain rights in this software.
#
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this software; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
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
        i_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("sample_rate"), pmt.from_float(1000.0))
        in_pdu = pmt.cons(i_meta, pmt.init_u8vector(len(i_data), i_data))

        e_data = [1, 0, 0, 0] * 5
        e_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("sample_rate"), pmt.from_float(1000.0))
        e_pdu = pmt.cons(e_meta, pmt.init_u8vector(len(e_data), e_data))

        self.tb.start()
        time.sleep(.01)
        self.emitter.emit(in_pdu)
        time.sleep(.1)
        self.tb.stop()
        self.tb.wait()

        print("test_001:")
        print("pdu expected: " + repr(pmt.car(e_pdu)))
        print("pdu got:      " + repr(pmt.car(self.debug.get_message(0))))
        print("data expected: " + repr(pmt.to_python(pmt.cdr(e_pdu))))
        print("data got:      " + repr(pmt.to_python(pmt.cdr(self.debug.get_message(0)))))
        print

        self.assertTrue(pmt.equal(self.debug.get_message(0), e_pdu))

    def test_001_float(self):
        '''
        uint8_t input data, no decimation, no filter
        '''
        self.dut = pdu_utils.pdu_fir_filter(1, [1.0])
        self.connectUp()

        i_data = [1, 0, 0, 0] * 5
        i_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("sample_rate"), pmt.from_float(1000.0))
        in_pdu = pmt.cons(i_meta, pmt.init_f32vector(len(i_data), i_data))

        e_data = [1, 0, 0, 0] * 5
        e_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("sample_rate"), pmt.from_float(1000.0))
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
        i_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("sample_rate"), pmt.from_float(1000.0))
        in_pdu = pmt.cons(i_meta, pmt.init_c32vector(len(i_data), i_data))

        e_data = [1, 0, 0, 0] * 5
        e_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("sample_rate"), pmt.from_float(1000.0))
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
        i_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("sample_rate"), pmt.from_float(1000.0))
        in_pdu = pmt.cons(i_meta, pmt.init_u8vector(len(i_data), i_data))

        e_data = [1, 0, 0, 0] * 5
        e_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("sample_rate"), pmt.from_float(1000.0))
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

    def test_002_float(self):
        '''
        uint8_t input data, no decimation, attenuation filter
        '''
        self.dut = pdu_utils.pdu_fir_filter(1, [0.5])
        self.connectUp()

        i_data = [2, 0, 0, 0] * 5
        i_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("sample_rate"), pmt.from_float(1000.0))
        in_pdu = pmt.cons(i_meta, pmt.init_f32vector(len(i_data), i_data))

        e_data = [1, 0, 0, 0] * 5
        e_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("sample_rate"), pmt.from_float(1000.0))
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

    def test_002_c32(self):
        '''
        uint8_t input data, no decimation, attenuation filter
        '''
        self.dut = pdu_utils.pdu_fir_filter(1, [0.5])
        self.connectUp()

        i_data = [2, 0, 0, 0] * 5
        i_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("sample_rate"), pmt.from_float(1000.0))
        in_pdu = pmt.cons(i_meta, pmt.init_c32vector(len(i_data), i_data))

        e_data = [1, 0, 0, 0] * 5
        e_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("sample_rate"), pmt.from_float(1000.0))
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

    def test_003_uint8(self):
        '''
        uint8_t input data, decimation, no filter
        '''
        self.dut = pdu_utils.pdu_fir_filter(2, [1.0])
        self.connectUp()

        i_data = [1, 0, 0, 0] * 5
        i_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("sample_rate"), pmt.from_float(1000.0))
        in_pdu = pmt.cons(i_meta, pmt.init_u8vector(len(i_data), i_data))

        e_data = [1, 0] * 5
        e_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("sample_rate"), pmt.from_float(500.0))
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

    def test_003_float(self):
        '''
        uint8_t input data, decimation, no filter
        '''
        self.dut = pdu_utils.pdu_fir_filter(2, [1.0])
        self.connectUp()

        i_data = [1, 0, 0, 0] * 5
        i_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("sample_rate"), pmt.from_float(1000.0))
        in_pdu = pmt.cons(i_meta, pmt.init_f32vector(len(i_data), i_data))

        e_data = [1, 0] * 5
        e_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("sample_rate"), pmt.from_float(500.0))
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

    def test_003_c32(self):
        '''
        uint8_t input data, decimation, no filter
        '''
        self.dut = pdu_utils.pdu_fir_filter(2, [1.0])
        self.connectUp()

        i_data = [1, 0, 0, 0] * 5
        i_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("sample_rate"), pmt.from_float(1000.0))
        in_pdu = pmt.cons(i_meta, pmt.init_c32vector(len(i_data), i_data))

        e_data = [1, 0] * 5
        e_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("sample_rate"), pmt.from_float(500.0))
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


if __name__ == '__main__':
    gr_unittest.run(qa_pdu_fir_filter)
