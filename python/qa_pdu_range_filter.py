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

import pdu_utils_swig as pdu_utils
from gnuradio import gr, gr_unittest
from gnuradio import blocks
import pmt
import time


class qa_pdu_range_filter (gr_unittest.TestCase):

    def setUp(self):
        self.tb = gr.top_block()
        self.emitter = pdu_utils.message_emitter()
        self.debug = blocks.message_debug()

    def connectUp(self):
        self.tb.msg_connect((self.emitter, 'msg'), (self.dut, 'pdu_in'))
        self.tb.msg_connect((self.dut, 'pdu_out'), (self.debug, 'store'))

    def tearDown(self):
        self.tb = None

    def test_001_no_meta(self):

        self.dut = pdu_utils.pdu_range_filter(pmt.intern("start_time"), 1, 5, False)
        self.connectUp()

        in_data = [0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1]
        in_pdu = pmt.cons(pmt.make_dict(), pmt.init_u8vector(len(in_data), in_data))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        self.assertEqual(0, self.debug.num_messages())

    def test_002_normal_dropa(self):

        self.dut = pdu_utils.pdu_range_filter(pmt.intern("start_time"), 1, 5, False)
        self.connectUp()

        in_data = [0, 0, 0, 0, 0, 0]
        i_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("start_time"), pmt.from_double(0.0))
        in_pdu = pmt.cons(i_meta, pmt.init_u8vector(len(in_data), in_data))
        expected_data = [0, 0, 0, 0, 0, 0]
        e_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("start_time"), pmt.from_double(1.0))
        expected_pdu = pmt.cons(e_meta, pmt.init_u8vector(len(expected_data), expected_data))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        self.assertEqual(0, self.debug.num_messages())

    def test_002_normal_dropb(self):

        self.dut = pdu_utils.pdu_range_filter(pmt.intern("start_time"), 1, 5, False)
        self.connectUp()

        in_data = [0, 0, 0, 0, 0, 0]
        i_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("start_time"), pmt.from_double(6.0))
        in_pdu = pmt.cons(i_meta, pmt.init_u8vector(len(in_data), in_data))
        expected_data = [0, 0, 0, 0, 0, 0]
        e_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("start_time"), pmt.from_double(1.0))
        expected_pdu = pmt.cons(e_meta, pmt.init_u8vector(len(expected_data), expected_data))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        self.assertEqual(0, self.debug.num_messages())

    def test_003_normal_passa(self):

        self.dut = pdu_utils.pdu_range_filter(pmt.intern("start_time"), 1, 5, False)
        self.connectUp()

        in_data = [0, 0, 0, 0, 0, 0]
        i_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("start_time"), pmt.from_double(1.0))
        in_pdu = pmt.cons(i_meta, pmt.init_u8vector(len(in_data), in_data))
        expected_data = [0, 0, 0, 0, 0, 0]
        e_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("start_time"), pmt.from_double(1.0))
        expected_pdu = pmt.cons(e_meta, pmt.init_u8vector(len(expected_data), expected_data))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        self.assertEqual(1, self.debug.num_messages())
        self.assertTrue(pmt.equal(self.debug.get_message(0), expected_pdu))

    def test_003_normal_passb(self):

        self.dut = pdu_utils.pdu_range_filter(pmt.intern("start_time"), 1, 5, False)
        self.connectUp()

        in_data = [0, 0, 0, 0, 0, 0]
        i_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("start_time"), pmt.from_double(3.0))
        in_pdu = pmt.cons(i_meta, pmt.init_u8vector(len(in_data), in_data))
        expected_data = [0, 0, 0, 0, 0, 0]
        e_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("start_time"), pmt.from_double(3.0))
        expected_pdu = pmt.cons(e_meta, pmt.init_u8vector(len(expected_data), expected_data))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        self.assertEqual(1, self.debug.num_messages())
        self.assertTrue(pmt.equal(self.debug.get_message(0), expected_pdu))

    def test_003_normal_passc(self):

        self.dut = pdu_utils.pdu_range_filter(pmt.intern("start_time"), 1, 5, False)
        self.connectUp()

        in_data = [0, 0, 0, 0, 0, 0]
        i_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("start_time"), pmt.from_double(5.0))
        in_pdu = pmt.cons(i_meta, pmt.init_u8vector(len(in_data), in_data))
        expected_data = [0, 0, 0, 0, 0, 0]
        e_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("start_time"), pmt.from_double(5.0))
        expected_pdu = pmt.cons(e_meta, pmt.init_u8vector(len(expected_data), expected_data))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        self.assertEqual(1, self.debug.num_messages())
        self.assertTrue(pmt.equal(self.debug.get_message(0), expected_pdu))

    ####################################

    def test_004_invert_passa(self):

        self.dut = pdu_utils.pdu_range_filter(pmt.intern("start_time"), 1, 5, True)
        self.connectUp()

        in_data = [0, 0, 0, 0, 0, 0]
        i_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("start_time"), pmt.from_double(0.0))
        in_pdu = pmt.cons(i_meta, pmt.init_u8vector(len(in_data), in_data))
        expected_data = [0, 0, 0, 0, 0, 0]
        e_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("start_time"), pmt.from_double(0.0))
        expected_pdu = pmt.cons(e_meta, pmt.init_u8vector(len(expected_data), expected_data))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        self.assertEqual(1, self.debug.num_messages())
        self.assertTrue(pmt.equal(self.debug.get_message(0), expected_pdu))

    def test_004_invert_passb(self):

        self.dut = pdu_utils.pdu_range_filter(pmt.intern("start_time"), 1, 5, True)
        self.connectUp()

        in_data = [0, 0, 0, 0, 0, 0]
        i_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("start_time"), pmt.from_double(6.0))
        in_pdu = pmt.cons(i_meta, pmt.init_u8vector(len(in_data), in_data))
        expected_data = [0, 0, 0, 0, 0, 0]
        e_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("start_time"), pmt.from_double(6.0))
        expected_pdu = pmt.cons(e_meta, pmt.init_u8vector(len(expected_data), expected_data))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        self.assertEqual(1, self.debug.num_messages())
        self.assertTrue(pmt.equal(self.debug.get_message(0), expected_pdu))

    def test_005_invert_dropa(self):

        self.dut = pdu_utils.pdu_range_filter(pmt.intern("start_time"), 1, 5, True)
        self.connectUp()

        in_data = [0, 0, 0, 0, 0, 0]
        i_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("start_time"), pmt.from_double(1.0))
        in_pdu = pmt.cons(i_meta, pmt.init_u8vector(len(in_data), in_data))
        expected_data = [0, 0, 0, 0, 0, 0]
        e_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("start_time"), pmt.from_double(1.0))
        expected_pdu = pmt.cons(e_meta, pmt.init_u8vector(len(expected_data), expected_data))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        self.assertEqual(0, self.debug.num_messages())

    def test_005_invert_dropb(self):

        self.dut = pdu_utils.pdu_range_filter(pmt.intern("start_time"), 1, 5, True)
        self.connectUp()

        in_data = [0, 0, 0, 0, 0, 0]
        i_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("start_time"), pmt.from_double(3.0))
        in_pdu = pmt.cons(i_meta, pmt.init_u8vector(len(in_data), in_data))
        expected_data = [0, 0, 0, 0, 0, 0]
        e_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("start_time"), pmt.from_double(3.0))
        expected_pdu = pmt.cons(e_meta, pmt.init_u8vector(len(expected_data), expected_data))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        self.assertEqual(0, self.debug.num_messages())

    def test_005_invert_dropc(self):

        self.dut = pdu_utils.pdu_range_filter(pmt.intern("start_time"), 1, 5, True)
        self.connectUp()

        in_data = [0, 0, 0, 0, 0, 0]
        i_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("start_time"), pmt.from_double(5.0))
        in_pdu = pmt.cons(i_meta, pmt.init_u8vector(len(in_data), in_data))
        expected_data = [0, 0, 0, 0, 0, 0]
        e_meta = pmt.dict_add(pmt.make_dict(), pmt.intern("start_time"), pmt.from_double(5.0))
        expected_pdu = pmt.cons(e_meta, pmt.init_u8vector(len(expected_data), expected_data))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        self.assertEqual(0, self.debug.num_messages())


if __name__ == '__main__':
    gr_unittest.run(qa_pdu_range_filter)
