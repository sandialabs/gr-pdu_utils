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

class qa_extract_metadata (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()
        self.emitter = pdu_utils.message_emitter()
        self.emd = pdu_utils.extract_metadata(pmt.intern("test_key"), 1, 0)
        self.debug = blocks.message_debug()
        self.tb.msg_connect((self.emitter, 'msg'), (self.emd, 'dict'))
        self.tb.msg_connect((self.emd, 'msg'), (self.debug, 'store'))
        self.base_dict = pmt.make_dict()
        self.base_dict = pmt.dict_add(self.base_dict, pmt.intern("key1"), pmt.intern("value1"))
        self.base_dict = pmt.dict_add(self.base_dict, pmt.intern("key2"), pmt.intern("value2"))
        self.base_dict = pmt.dict_add(self.base_dict, pmt.intern("uint64_key"), pmt.from_uint64(1234567))
        self.base_dict = pmt.dict_add(self.base_dict, pmt.intern("double_key"), pmt.from_double(1.234567))

    def tearDown (self):
        self.tb = None


    def test_001_int (self):
        self.emd.set_key(pmt.intern("int"))
        self.emd.set_scale(1)
        self.emd.set_offset(1)

        in_msg = pmt.dict_add(self.base_dict, pmt.intern("int"), pmt.from_long(-4))
        expected_msg = pmt.cons(pmt.intern("int"), pmt.from_long(-3))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_msg)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        self.assertTrue(pmt.equal(self.debug.get_message(0), expected_msg))

    def test_001_int_scalea (self):
        self.emd.set_key(pmt.intern("int"))
        self.emd.set_scale(2)
        self.emd.set_offset(0)

        in_msg = pmt.dict_add(self.base_dict, pmt.intern("int"), pmt.from_long(-4))
        expected_msg = pmt.cons(pmt.intern("int"), pmt.from_long(-8))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_msg)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        self.assertTrue(pmt.equal(self.debug.get_message(0), expected_msg))
        
    def test_001_int_scaleb (self):
        self.emd.set_key(pmt.intern("int"))
        self.emd.set_scale(-2)
        self.emd.set_offset(0)

        in_msg = pmt.dict_add(self.base_dict, pmt.intern("int"), pmt.from_long(-4))
        expected_msg = pmt.cons(pmt.intern("int"), pmt.from_long(8))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_msg)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        self.assertTrue(pmt.equal(self.debug.get_message(0), expected_msg))

    def test_002_float (self):
        self.emd.set_key(pmt.intern("float"))
        self.emd.set_scale(0.5)
        self.emd.set_offset(900)

        in_msg = pmt.dict_add(self.base_dict, pmt.intern("float"), pmt.from_float(-493.5))
        expected_msg = pmt.cons(pmt.intern("float"), pmt.from_float(-493.5/2 + 900))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_msg)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        self.assertTrue(pmt.equal(self.debug.get_message(0), expected_msg))


    def test_003_pdu (self):
        self.emd.set_key(pmt.intern("key2"))
        self.emd.set_scale(3.333)
        self.emd.set_offset(2)

        in_msg = pmt.cons(self.base_dict, pmt.init_f32vector(3,[1.1,2.2,3.3]))
        expected_msg = pmt.cons(pmt.intern("key2"), pmt.intern("value2"))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_msg)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        self.assertTrue(pmt.equal(self.debug.get_message(0), expected_msg))
        
    def test_004_uint64 (self):
        self.emd.set_key(pmt.intern("uint64_key"))
        self.emd.set_scale(1)
        self.emd.set_offset(0)

        in_msg = self.base_dict
        expected_msg = pmt.cons(pmt.intern("uint64_key"), pmt.from_uint64(1234567))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_msg)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        self.assertTrue(pmt.equal(self.debug.get_message(0), expected_msg))
        
    def test_004_uint64_offseta (self):
        self.emd.set_key(pmt.intern("uint64_key"))
        self.emd.set_scale(1)
        self.emd.set_offset(10)

        in_msg = self.base_dict
        expected_msg = pmt.cons(pmt.intern("uint64_key"), pmt.from_uint64(1234577))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_msg)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        self.assertTrue(pmt.equal(self.debug.get_message(0), expected_msg))
        
    def test_004_uint64_offsetb (self):
        self.emd.set_key(pmt.intern("uint64_key"))
        self.emd.set_scale(1)
        self.emd.set_offset(-10)

        in_msg = self.base_dict
        expected_msg = pmt.cons(pmt.intern("uint64_key"), pmt.from_uint64(1234557))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_msg)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        self.assertTrue(pmt.equal(self.debug.get_message(0), expected_msg))
        
    def test_004_uint64_scalea (self):
        self.emd.set_key(pmt.intern("uint64_key"))
        self.emd.set_scale(2)
        self.emd.set_offset(0)

        in_msg = self.base_dict
        expected_msg = pmt.cons(pmt.intern("uint64_key"), pmt.from_uint64(2469134))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_msg)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        self.assertTrue(pmt.equal(self.debug.get_message(0), expected_msg))
        
    
        
    def test_005_notfound (self):
        self.emd.set_key(pmt.intern("barf"))
        self.emd.set_scale(1)
        self.emd.set_offset(1)

        in_msg = self.base_dict
        

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_msg)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        self.assertEqual( 0, self.debug.num_messages() )


if __name__ == '__main__':
    gr_unittest.run(qa_extract_metadata)
