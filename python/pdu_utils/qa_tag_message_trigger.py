#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright 2018-2021 National Technology & Engineering Solutions of Sandia, LLC
# (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
# retains certain rights in this software.
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

try:
  from gnuradio import pdu_utils
except ImportError:
    import os
    import sys
    dirname, filename = os.path.split(os.path.abspath(__file__))
    sys.path.append(os.path.join(dirname, "bindings"))
    from gnuradio import pdu_utils

from gnuradio import gr, gr_unittest
from gnuradio import blocks
import pmt
import time


class qa_tag_message_trigger (gr_unittest.TestCase):

    def setUp (self):
        pass

    def tearDown (self):
        pass

    def test_001_basic (self):
        self.tb = gr.top_block ()

        trig_key = pmt.intern("TRIGGER")
        tags = []
        tags.append(gr.tag_utils.python_to_tag((5, trig_key, pmt.PMT_T)))  # should trigger
        tags.append(gr.tag_utils.python_to_tag((6, trig_key, pmt.PMT_T)))  # should trigger
        msg = pmt.cons(pmt.dict_add(pmt.make_dict(), pmt.intern("K"), pmt.intern("V")), pmt.init_u8vector(4,[48,49,50,51,]))
        src = blocks.vector_source_f(range(20), False, 1, tags)
        tmt = pdu_utils.tag_message_trigger_f(trig_key, pmt.PMT_NIL, msg, 0, 0.0, 0.0, 0.0, False)
        debug = blocks.message_debug()
        self.tb.connect(src, tmt)

        self.tb.msg_connect((tmt, 'msg'), (debug, 'store'))
        self.tb.run ()

        self.assertEqual(debug.num_messages(),2)
        self.assertTrue(pmt.eqv(msg, debug.get_message(0)))

        self.tb = None


    def test_002_holdoff (self):
        self.tb = gr.top_block ()

        trig_key = pmt.intern("TRIGGER")
        tags = []
        tags.append(gr.tag_utils.python_to_tag((5, trig_key, pmt.PMT_T)))  # should trigger
        tags.append(gr.tag_utils.python_to_tag((6, trig_key, pmt.PMT_T)))
        tags.append(gr.tag_utils.python_to_tag((9, trig_key, pmt.PMT_T)))  # should trigger
        tags.append(gr.tag_utils.python_to_tag((10, trig_key, pmt.PMT_T)))
        tags.append(gr.tag_utils.python_to_tag((11, trig_key, pmt.PMT_T)))
        tags.append(gr.tag_utils.python_to_tag((12, trig_key, pmt.PMT_T)))
        tags.append(gr.tag_utils.python_to_tag((13, trig_key, pmt.PMT_T)))  # should trigger
        msg = pmt.cons(pmt.dict_add(pmt.make_dict(), pmt.intern("K1"), pmt.intern("V1")), pmt.init_u8vector(4,[48,49,50,51,]))
        src = blocks.vector_source_f(range(20), False, 1, tags)
        tmt = pdu_utils.tag_message_trigger_f(trig_key, pmt.PMT_NIL, msg, 3, 0.0, 0.0, 0.0, False)
        debug = blocks.message_debug()
        self.tb.connect(src, tmt)

        self.tb.msg_connect((tmt, 'msg'), (debug, 'store'))
        self.tb.run ()

        self.assertEqual(debug.num_messages(),3)
        self.assertTrue(pmt.eqv(msg, debug.get_message(0)))
        self.assertTrue(pmt.eqv(msg, debug.get_message(1)))

        self.tb = None


    def test_003_holdoff2 (self):
        self.tb = gr.top_block ()

        trig_key = pmt.intern("TRIGGER")
        tags = []
        tags.append(gr.tag_utils.python_to_tag((5, trig_key, pmt.PMT_T)))
        tags.append(gr.tag_utils.python_to_tag((10005, trig_key, pmt.PMT_T)))  # should trigger
        tags.append(gr.tag_utils.python_to_tag((19994, trig_key, pmt.PMT_T)))
        tags.append(gr.tag_utils.python_to_tag((19995, trig_key, pmt.PMT_T)))
        tags.append(gr.tag_utils.python_to_tag((19996, trig_key, pmt.PMT_T)))  # should trigger
        msg = pmt.cons(pmt.dict_add(pmt.make_dict(), pmt.intern("K1"), pmt.intern("V1")), pmt.init_u8vector(4,[48,49,50,51,]))
        src = blocks.vector_source_f(range(20000), False, 1, tags)
        tmt = pdu_utils.tag_message_trigger_f(trig_key, pmt.PMT_NIL, msg, 9990, 0.0, 0.0, 0.0, False)
        debug = blocks.message_debug()
        self.tb.connect(src, tmt)

        self.tb.msg_connect((tmt, 'msg'), (debug, 'store'))
        self.tb.run ()

        self.assertEqual(debug.num_messages(),2)
        self.assertTrue(pmt.eqv(msg, debug.get_message(0)))
        self.assertTrue(pmt.eqv(msg, debug.get_message(1)))

        self.tb = None


    def test_004_holdoff_arming (self):
        self.tb = gr.top_block ()

        trig_key = pmt.intern("TRIGGER")
        arm_key = pmt.intern("ARM")
        tags = []
        tags.append(gr.tag_utils.python_to_tag((5, trig_key, pmt.PMT_T)))
        tags.append(gr.tag_utils.python_to_tag((20, trig_key, pmt.PMT_T)))
        tags.append(gr.tag_utils.python_to_tag((21, arm_key, pmt.PMT_T)))
        tags.append(gr.tag_utils.python_to_tag((22, trig_key, pmt.PMT_T)))  # should trigger
        tags.append(gr.tag_utils.python_to_tag((30, arm_key, pmt.PMT_T)))
        tags.append(gr.tag_utils.python_to_tag((32, trig_key, pmt.PMT_T)))
        tags.append(gr.tag_utils.python_to_tag((33, trig_key, pmt.PMT_T)))  # should trigger
        tags.append(gr.tag_utils.python_to_tag((50, arm_key, pmt.PMT_T)))
        tags.append(gr.tag_utils.python_to_tag((60, trig_key, pmt.PMT_T)))
        tags.append(gr.tag_utils.python_to_tag((70, arm_key, pmt.PMT_T)))
        tags.append(gr.tag_utils.python_to_tag((79, trig_key, pmt.PMT_T)))  # should trigger
        msg = pmt.cons(pmt.dict_add(pmt.make_dict(), pmt.intern("K1"), pmt.intern("V1")), pmt.init_u8vector(4,[48,49,50,51,]))
        src = blocks.vector_source_f(range(100), False, 1, tags)
        tmt = pdu_utils.tag_message_trigger_f(trig_key, arm_key, msg, 10, 0.0, 0.0, 0.0, False)
        debug = blocks.message_debug()
        self.tb.connect(src, tmt)

        self.tb.msg_connect((tmt, 'msg'), (debug, 'store'))
        self.tb.run ()

        self.assertEqual(debug.num_messages(),3)
        self.assertTrue(pmt.eqv(msg, debug.get_message(0)))
        self.assertTrue(pmt.eqv(msg, debug.get_message(1)))

        self.tb = None


if __name__ == '__main__':
    #z=raw_input("press enter")
    gr_unittest.run(qa_tag_message_trigger)
