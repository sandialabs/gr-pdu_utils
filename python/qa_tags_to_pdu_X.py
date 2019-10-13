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

from gnuradio import gr, gr_unittest
from gnuradio import blocks
import pdu_utils_swig as pdu_utils
import pmt
import numpy
import time

class qa_tags_to_pdu_X (gr_unittest.TestCase):

    def setUp (self):
        pass

    def tearDown (self):
        pass

    def test_001_simple (self):
        self.tb = gr.top_block ()
        start_time = 0.1
        sob_tag = gr.tag_utils.python_to_tag((34, pmt.intern("SOB"), pmt.PMT_T, pmt.intern("src")))
        eob_tag = gr.tag_utils.python_to_tag((34+(8*31), pmt.intern("EOB"), pmt.PMT_T, pmt.intern("src")))
        vs = blocks.vector_source_s(range(350), False, 1, [sob_tag, eob_tag])
        t2p = pdu_utils.tags_to_pdu_s(pmt.intern('SOB'), pmt.intern('EOB'), 1024, 512000, ([]), False, 0, start_time)
        t2p.set_eob_parameters(8, 0)
        dbg = blocks.message_debug()
        self.tb.connect(vs, t2p)
        self.tb.msg_connect((t2p, 'pdu_out'), (dbg, 'store'))
        expected_vec = pmt.init_s16vector((8*31), range(34,34+(8*31)))
        expected_time = start_time + (34 / 512000.0)

        self.tb.run ()

        self.assertEqual(dbg.num_messages(), 1)
        self.assertTrue(pmt.equal(pmt.cdr(dbg.get_message(0)), expected_vec))
        time_tuple1 = pmt.dict_ref(pmt.car(dbg.get_message(0)), pmt.intern("burst_time"), pmt.PMT_NIL)
        self.assertAlmostEqual(pmt.to_uint64(pmt.tuple_ref(time_tuple1,0)) + pmt.to_double(pmt.tuple_ref(time_tuple1,1)), expected_time)

        self.tb = None


    def test_002_secondSOB (self):
        self.tb = gr.top_block ()
        start_time = 4.999999999
        sob_tag = gr.tag_utils.python_to_tag((34, pmt.intern("SOB"), pmt.PMT_T, pmt.intern("src")))
        sob_tag2 = gr.tag_utils.python_to_tag((51, pmt.intern("SOB"), pmt.PMT_T, pmt.intern("src")))
        eob_tag = gr.tag_utils.python_to_tag((51+(8*26), pmt.intern("EOB"), pmt.PMT_T, pmt.intern("src")))
        vs = blocks.vector_source_s(range(350), False, 1, [sob_tag, sob_tag2, eob_tag])
        t2p = pdu_utils.tags_to_pdu_s(pmt.intern('SOB'), pmt.intern('EOB'), 1024, 460800, ([]), False, 0, start_time)
        t2p.set_eob_parameters(8, 0)
        dbg = blocks.message_debug()
        self.tb.connect(vs, t2p)
        self.tb.msg_connect((t2p, 'pdu_out'), (dbg, 'store'))
        expected_vec = pmt.init_s16vector((8*26), range(51,51+(8*26)))
        expected_time = start_time + (51 / 460800.0)

        self.tb.run ()

        self.assertEqual(dbg.num_messages(), 1)
        self.assertTrue(pmt.equal(pmt.cdr(dbg.get_message(0)), expected_vec))
        time_tuple1 = pmt.dict_ref(pmt.car(dbg.get_message(0)), pmt.intern("burst_time"), pmt.PMT_NIL)
        self.assertAlmostEqual(pmt.to_uint64(pmt.tuple_ref(time_tuple1,0)) + pmt.to_double(pmt.tuple_ref(time_tuple1,1)), expected_time)

        self.tb = None


    def test_003_double_eob_rej_tt_update (self):
        self.tb = gr.top_block ()
        start_time = 0.0
        sob_tag = gr.tag_utils.python_to_tag((51, pmt.intern("SOB"), pmt.PMT_T, pmt.intern("src")))
        eob_tag = gr.tag_utils.python_to_tag((51+(8*11), pmt.intern("EOB"), pmt.PMT_T, pmt.intern("src")))
        time_tuple = pmt.make_tuple(pmt.from_uint64(4), pmt.from_double(0.125), pmt.from_uint64(10000000), pmt.from_double(4000000.0))
        time_tag = gr.tag_utils.python_to_tag((360, pmt.intern("rx_time"), time_tuple, pmt.intern("src")))
        sob_tag2 = gr.tag_utils.python_to_tag((400, pmt.intern("SOB"), pmt.PMT_T, pmt.intern("src")))
        eob_tag2e = gr.tag_utils.python_to_tag((409, pmt.intern("EOB"), pmt.PMT_T, pmt.intern("src")))
        eob_tag2 = gr.tag_utils.python_to_tag((416, pmt.intern("EOB"), pmt.PMT_T, pmt.intern("src")))
        vs = blocks.vector_source_s(range(500), False, 1, [sob_tag, eob_tag, time_tag, sob_tag2, eob_tag2e, eob_tag2])
        t2p = pdu_utils.tags_to_pdu_s(pmt.intern('SOB'), pmt.intern('EOB'), 1024, 1000000, ([]), False, 0, start_time)
        t2p.set_eob_parameters(8, 0)
        dbg = blocks.message_debug()
        self.tb.connect(vs, t2p)
        self.tb.msg_connect((t2p, 'pdu_out'), (dbg, 'store'))
        expected_vec1 = pmt.init_s16vector((8*11), range(51,51+(8*11)))
        expected_vec2 = pmt.init_s16vector(16, range(400,416))
        expected_time1 = start_time + (51 / 1000000.0)
        expected_time2 = 4.125 + ((400-360) / 1000000.0)

        self.tb.run ()

        self.assertEqual(dbg.num_messages(), 2)
        self.assertTrue(pmt.equal(pmt.cdr(dbg.get_message(0)), expected_vec1))
        self.assertTrue(pmt.equal(pmt.cdr(dbg.get_message(1)), expected_vec2))
        time_tuple1 = pmt.dict_ref(pmt.car(dbg.get_message(0)), pmt.intern("burst_time"), pmt.PMT_NIL)
        time_tuple2 = pmt.dict_ref(pmt.car(dbg.get_message(1)), pmt.intern("burst_time"), pmt.PMT_NIL)
        self.assertAlmostEqual(pmt.to_uint64(pmt.tuple_ref(time_tuple1,0)) + pmt.to_double(pmt.tuple_ref(time_tuple1,1)), expected_time1)
        self.assertAlmostEqual(pmt.to_uint64(pmt.tuple_ref(time_tuple2,0)) + pmt.to_double(pmt.tuple_ref(time_tuple2,1)), expected_time2)

        self.tb = None

    def test_004_boost_time (self):
        self.tb = gr.top_block ()
        start_time = 0.1
        sob_tag = gr.tag_utils.python_to_tag((34, pmt.intern("SOB"), pmt.PMT_T, pmt.intern("src")))
        eob_tag = gr.tag_utils.python_to_tag((34+(8*31), pmt.intern("EOB"), pmt.PMT_T, pmt.intern("src")))
        vs = blocks.vector_source_s(range(350), False, 1, [sob_tag, eob_tag])
        t2p = pdu_utils.tags_to_pdu_s(pmt.intern('SOB'), pmt.intern('EOB'), 1024, 512000, ([]), False, 0, start_time)
        t2p.enable_time_debug(True)
        t2p.set_eob_parameters(8, 0)
        dbg = blocks.message_debug()
        td = pdu_utils.time_delta("TIME CHECKER")

        self.tb.connect(vs, t2p)
        self.tb.msg_connect((t2p, 'pdu_out'), (dbg, 'store'))
        self.tb.msg_connect((t2p, 'pdu_out'), (td, 'pdu_in'))
        expected_vec = pmt.init_s16vector((8*31), range(34,34+(8*31)))
        expected_time = start_time + (34 / 512000.0)
        ts = time.time()
        self.tb.run ()

        self.assertEqual(dbg.num_messages(), 1)
        self.assertTrue(pmt.equal(pmt.cdr(dbg.get_message(0)), expected_vec))
        time_tuple1 = pmt.dict_ref(pmt.car(dbg.get_message(0)), pmt.intern("burst_time"), pmt.PMT_NIL)
        self.assertAlmostEqual(pmt.to_uint64(pmt.tuple_ref(time_tuple1,0)) + pmt.to_double(pmt.tuple_ref(time_tuple1,1)), expected_time)
        wct = pmt.to_double(pmt.dict_ref(pmt.car(dbg.get_message(0)), pmt.intern("wall_clock_time"), pmt.PMT_NIL))
        self.assertTrue((wct - ts) < 1.0)

        self.tb = None


# TODO: add more tests:
#   - test ability to correctly handle max-length pdus
#   - test callbacks
#   - test message setting interface
#   - test time-tracking tags


if __name__ == '__main__':
    gr_unittest.run(qa_tags_to_pdu_X)
