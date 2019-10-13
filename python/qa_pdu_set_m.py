#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright 2018 National Technology & Engineering Solutions of Sandia, LLC (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government retains certain rights in this software.
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

import pdu_utils_swig as pdu_utils
from gnuradio import gr, gr_unittest
from gnuradio import blocks
import pmt
import time


class qa_pdu_set_m (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()
        self.emitter = pdu_utils.message_emitter()
        self.set = pdu_utils.pdu_set_m(pmt.intern('num'), pmt.from_long(4))
        self.debug = blocks.message_debug()
        self.tb.msg_connect((self.emitter, 'msg'), (self.set, 'pdu_in'))
        self.tb.msg_connect((self.set, 'pdu_out'), (self.debug, 'store'))

    def tearDown (self):
        self.tb = None

    def test_001_null_dict (self):
        in_data = [0, 0, 0, 0]
        in_meta = pmt.make_dict()
        expected_meta = pmt.dict_add(in_meta, pmt.intern('num'), pmt.from_long(4))
        in_pdu = pmt.cons(in_meta, pmt.init_u8vector(len(in_data), in_data))
        expected_pdu = pmt.cons(expected_meta, pmt.init_u8vector(len(in_data), in_data))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(pmt.intern("MALFORMED PDU"))
        time.sleep(.001)
        self.emitter.emit(pmt.cons(pmt.from_long(0), pmt.PMT_T))
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        self.assertTrue(pmt.equal(self.debug.get_message(0), expected_pdu))


    def test_002_callbacks (self):
        in_data = [0, 0, 0, 0]
        in_meta = pmt.make_dict()
        expected_meta1 = pmt.dict_add(in_meta, pmt.intern('num'), pmt.from_long(4))
        expected_meta2 = pmt.dict_add(pmt.dict_add(in_meta, pmt.intern('num'), pmt.from_long(4)), pmt.intern('name'), pmt.intern('param1'))
        expected_meta3 = pmt.dict_add(pmt.dict_add(in_meta, pmt.intern('name'), pmt.intern('param1')), pmt.intern('num'), pmt.from_long(1))
        in_pdu = pmt.cons(in_meta, pmt.init_u8vector(len(in_data), in_data))
        expected_pdu1 = pmt.cons(expected_meta1, pmt.init_u8vector(len(in_data), in_data))
        expected_pdu2 = pmt.cons(expected_meta2, pmt.init_u8vector(len(in_data), in_data))
        expected_pdu3 = pmt.cons(expected_meta3, pmt.init_u8vector(len(in_data), in_data))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(pmt.intern("MALFORMED PDU"))
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.001)
        self.set.set_key(pmt.intern('name'))
        self.set.set_val(pmt.intern('param1'))
        time.sleep(.001)
        self.emitter.emit(self.debug.get_message(0))
        time.sleep(.001)
        self.set.set_kv(pmt.intern('num'), pmt.from_long(1))
        time.sleep(.001)
        self.emitter.emit(self.debug.get_message(1))
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        self.assertTrue(pmt.equal(self.debug.get_message(0), expected_pdu1))
        self.assertTrue(pmt.equal(self.debug.get_message(1), expected_pdu2))
        self.assertTrue(pmt.equal(self.debug.get_message(2), expected_pdu3))



if __name__ == '__main__':
    gr_unittest.run(qa_pdu_set_m)
