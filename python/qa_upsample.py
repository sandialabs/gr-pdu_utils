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

class qa_upsample (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()
        self.emitter = pdu_utils.message_emitter()
        self.upsample = pdu_utils.upsample(4, False)
        self.debug = blocks.message_debug()
        self.tb.msg_connect((self.emitter, 'msg'), (self.upsample, 'pdu_in'))
        self.tb.msg_connect((self.upsample, 'pdu_out'), (self.debug, 'store'))

    def tearDown (self):
        self.tb = None

    def test_001_n4_u8 (self):
        in_data = [0, 1, 2, 4, 8, 16, 32]
        expected_data1 = [0, 0, 0, 0, 1, 0, 0, 0, 2, 0, 0, 0, 4, 0, 0, 0, 8, 0, 0, 0, 16, 0, 0, 0, 32, 0, 0, 0]
        expected_data2 = [0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 4, 4, 4, 4, 8, 8, 8, 8, 16, 16, 16, 16, 32, 32, 32, 32]
        in_pdu = pmt.cons(pmt.make_dict(), pmt.init_u8vector(len(in_data), in_data))
        expected_pdu1 = pmt.cons(pmt.make_dict(), pmt.init_u8vector(len(expected_data1), expected_data1))
        expected_pdu2 = pmt.cons(pmt.make_dict(), pmt.init_u8vector(len(expected_data2), expected_data2))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(pmt.intern("MALFORMED PDU"))
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.001)
        self.upsample.set_repeat(True)
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.05)
        self.tb.stop()
        self.tb.wait()

        self.assertTrue(pmt.equal(self.debug.get_message(0), expected_pdu1))
        self.assertTrue(pmt.equal(self.debug.get_message(1), expected_pdu2))


    def test_002_n1_c32 (self):
        self.upsample.set_n(1)
        self.upsample.set_repeat(False)

        in_data = [8+1j, 16-1j, 32j]
        in_pdu = pmt.cons(pmt.make_dict(), pmt.init_c32vector(len(in_data), in_data))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.001)
        self.upsample.set_repeat(True)
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.05)
        self.tb.stop()
        self.tb.wait()
        self.assertTrue(pmt.equal(self.debug.get_message(0), in_pdu))
        self.assertTrue(pmt.equal(self.debug.get_message(1), in_pdu))

    def test_003_change_f32 (self):
        self.upsample.set_n(1)
        self.upsample.set_repeat(True)

        in_data = [0, 1, 2, 4, 8, 16, 32]
        expected_data2 = [0, 0, 1, 1, 2, 2, 4, 4, 8, 8, 16, 16, 32, 32]
        expected_data3 = [0, 0, 0, 1, 1, 1, 2, 2, 2, 4, 4, 4, 8, 8, 8, 16, 16, 16, 32, 32, 32]
        in_pdu = pmt.cons(pmt.make_dict(), pmt.init_f32vector(len(in_data), in_data))
        expected_pdu1 = pmt.cons(pmt.make_dict(), pmt.init_f32vector(len(in_data), in_data))
        expected_pdu2 = pmt.cons(pmt.make_dict(), pmt.init_f32vector(len(expected_data2), expected_data2))
        expected_pdu3 = pmt.cons(pmt.make_dict(), pmt.init_f32vector(len(expected_data3), expected_data3))
        expected_pdu4 = pmt.cons(pmt.make_dict(), pmt.init_f32vector(len(expected_data3), expected_data3))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.001)
        self.upsample.set_n(2)
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.001)
        self.upsample.set_n(3)
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.001)
        self.upsample.set_n(0)
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.05)
        self.tb.stop()
        self.tb.wait()

        self.assertTrue(pmt.equal(self.debug.get_message(0), expected_pdu1))
        self.assertTrue(pmt.equal(self.debug.get_message(1), expected_pdu2))
        self.assertTrue(pmt.equal(self.debug.get_message(2), expected_pdu3))
        self.assertTrue(pmt.equal(self.debug.get_message(3), expected_pdu4))

    def test_004_n512_s32 (self):
        self.upsample.set_n(512)
        self.upsample.set_repeat(True)

        in_data = [99, 44]
        expected_data = [99,]*512 + [44,]*512
        in_pdu = pmt.cons(pmt.make_dict(), pmt.init_s32vector(len(in_data), in_data))
        expected_pdu = pmt.cons(pmt.make_dict(), pmt.init_s32vector(len(expected_data), expected_data))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(pmt.intern("MALFORMED PDU"))
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.05)
        self.tb.stop()
        self.tb.wait()

        self.assertTrue(pmt.equal(self.debug.get_message(0), expected_pdu))
        self.assertTrue(pmt.equal(self.debug.get_message(1), expected_pdu))




if __name__ == '__main__':
    gr_unittest.run(qa_upsample, "qa_upsample.xml")
