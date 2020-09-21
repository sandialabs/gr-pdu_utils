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


class qa_pdu_split (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_001_split (self):
        emitter = pdu_utils.message_emitter()
        split = pdu_utils.pdu_split()
        d1 = blocks.message_debug()
        d2 = blocks.message_debug()
        self.tb.msg_connect((emitter, 'msg'), (split, 'pdu_in'))
        self.tb.msg_connect((split, 'dict'), (d1, 'store'))
        self.tb.msg_connect((split, 'data'), (d2, 'store'))

        in_meta1 = pmt.dict_add(pmt.make_dict(), pmt.intern('num'), pmt.from_long(4))
        in_meta2 = pmt.dict_add(pmt.make_dict(), pmt.intern('n'), pmt.from_long(99))
        in_pdu = pmt.cons(in_meta1, pmt.init_u8vector(6, range(6)))

        self.tb.start()
        time.sleep(.001)
        emitter.emit(pmt.intern("MALFORMED PDU"))
        time.sleep(.001)
        emitter.emit(pmt.cons(pmt.PMT_NIL, pmt.init_u8vector(2, range(2))))
        time.sleep(.001)
        emitter.emit(pmt.cons(in_meta2, pmt.init_u8vector(0, [])))
        time.sleep(.001)
        emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        self.assertTrue(pmt.equal(d1.get_message(0), in_meta2))
        self.assertTrue(pmt.equal(d1.get_message(1), in_meta1))
        self.assertTrue(pmt.equal(d2.get_message(0), pmt.init_u8vector(2, range(2))))
        self.assertTrue(pmt.equal(d2.get_message(1), pmt.init_u8vector(6, range(6))))


    def test_002_pass_empty (self):
        emitter = pdu_utils.message_emitter()
        split = pdu_utils.pdu_split(True)
        d1 = blocks.message_debug()
        d2 = blocks.message_debug()
        self.tb.msg_connect((emitter, 'msg'), (split, 'pdu_in'))
        self.tb.msg_connect((split, 'dict'), (d1, 'store'))
        self.tb.msg_connect((split, 'data'), (d2, 'store'))

        in_meta1 = pmt.dict_add(pmt.make_dict(), pmt.intern('num'), pmt.from_long(4))
        in_meta2 = pmt.dict_add(pmt.make_dict(), pmt.intern('n'), pmt.from_long(99))
        in_pdu = pmt.cons(in_meta1, pmt.init_u8vector(6, range(6)))

        self.tb.start()
        time.sleep(.001)
        emitter.emit(pmt.intern("MALFORMED PDU"))
        time.sleep(.001)
        emitter.emit(pmt.cons(pmt.PMT_NIL, pmt.init_u8vector(2, range(2))))
        time.sleep(.001)
        emitter.emit(pmt.cons(in_meta2, pmt.init_u8vector(0, [])))
        time.sleep(.001)
        emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        self.assertTrue(pmt.equal(d1.get_message(0), pmt.PMT_NIL))
        self.assertTrue(pmt.equal(d1.get_message(1), in_meta2))
        self.assertTrue(pmt.equal(d1.get_message(2), in_meta1))
        self.assertTrue(pmt.equal(d2.get_message(0), pmt.init_u8vector(2, range(2))))
        self.assertTrue(pmt.equal(d2.get_message(1), pmt.init_u8vector(0, [])))
        self.assertTrue(pmt.equal(d2.get_message(2), pmt.init_u8vector(6, range(6))))


if __name__ == '__main__':
    gr_unittest.run(qa_pdu_split)
