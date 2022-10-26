#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright 2018, 2019, 2020, 2022 National Technology & Engineering Solutions of Sandia, LLC
# (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
# retains certain rights in this software.
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

from gnuradio import gr, gr_unittest
from gnuradio import blocks
try:
  from gnuradio import pdu_utils
except ImportError:
    import os
    import sys
    dirname, filename = os.path.split(os.path.abspath(__file__))
    sys.path.append(os.path.join(dirname, "bindings"))
    from gnuradio import pdu_utils

import numpy as np
import pmt
import time

class qa_access_code_to_pdu(gr_unittest.TestCase):

    def setUp(self):
        self.tb = gr.top_block()
        self.debug = blocks.message_debug()

    def connectUp(self):
        self.tb.connect((self.source, 0), (self.cut, 0))
        self.tb.msg_connect((self.cut, 'pdu_out'), (self.debug, 'store'))

    def tearDown(self):
        self.tb = None

    def makeMeta(self, reversed, pdu_num, bit_num):
        out = pmt.make_dict()
        out = pmt.dict_add(out, pdu_utils.PMTCONSTSTR__bit_reversed(), pmt.from_bool(reversed))
        out = pmt.dict_add(out, pdu_utils.PMTCONSTSTR__pdu_num(), pmt.from_uint64(pdu_num))
        out = pmt.dict_add(out, pdu_utils.PMTCONSTSTR__bit_index(), pmt.from_uint64(bit_num))
        return out

    # test the strict read mode and the discard syncword mode
    def test_001_t(self):
        syncword = [1, 0, 1, 1, 0, 1, 0, 0]
        data = [1, 1, 1] + [1, 0, 1, 1, 0, 1, 0, 0] * 6
        self.cut = pdu_utils.access_code_to_pdu('0b10110100','', 16, 0, pdu_utils.SYNC_DISCARD, pdu_utils.READ_STRICT)
        self.source = blocks.vector_source_b(data, False)
        self.connectUp()

        e_meta_0 = self.makeMeta(False, 0, 3)
        e_meta_1 = self.makeMeta(False, 1, 19)
        e_meta_2 = self.makeMeta(False, 2, 35)

        self.tb.run()

        self.assertTrue(pmt.equal(self.debug.get_message(0), pmt.cons(e_meta_0, pmt.init_u8vector(8, syncword))))
        self.assertTrue(pmt.equal(self.debug.get_message(1), pmt.cons(e_meta_1, pmt.init_u8vector(8, syncword))))
        self.assertTrue(pmt.equal(self.debug.get_message(2), pmt.cons(e_meta_2, pmt.init_u8vector(8, syncword))))

    # test the reset read mode and the fix syncword mode
    def test_002_t(self):
        data = [1, 1, 0] + [1, 1, 1, 1, 0, 1, 0, 0] + [0]*9 + [1, 0, 1, 1, 0, 0, 0, 0] + [0] * 45
        e_pdu_data = [1, 0, 1, 1, 0, 1, 0, 0] + [0] * 23 + [1]
        self.cut = pdu_utils.access_code_to_pdu('0xb4','0001', 32, 1, pdu_utils.SYNC_FIX, pdu_utils.READ_RESET)
        self.source = blocks.vector_source_b(data, False)
        self.connectUp()

        e_meta_0 = self.makeMeta(False, 0, 20)

        self.tb.run()

        self.assertTrue(pmt.equal(self.debug.get_message(0), pmt.cons(e_meta_0, pmt.init_u8vector(32, e_pdu_data))))

    # test the permissive read mode, the keep syncword mode, bit_reversal, and the set_ methods
    def test_003_t(self):
        data = [1, 0, 1, 1, 0, 1, 0, 0] + [0, 1, 0, 1] + [1, 1, 1, 1] + [1, 1, 1]
        data = data + [0, 1, 0, 0, 1, 0, 1, 0] + [1, 1, 1, 0] + [1, 1, 0, 0]
        data = data + [0, 1, 0, 0, 1, 0, 1, 1] + [0, 0, 1, 1] + [1, 0, 1, 1]
        data = data + [0, 1, 0, 0] + [1, 0, 1, 0] + [0, 0, 1, 0]
        e_pdu_data_0 = [1, 0, 1, 1, 0, 1, 0, 0] + [1, 1, 0, 0] + [0, 1, 0, 0]
        e_pdu_data_1 = [1, 0, 1, 1, 0, 1, 0, 0] + [1, 0, 1, 0] + [0, 0, 1, 0]
        self.cut = pdu_utils.access_code_to_pdu('10110100','0000', 16, 1, pdu_utils.SYNC_KEEP, pdu_utils.READ_PERMISSIVE)
        self.source = blocks.vector_source_b(data, False)
        self.connectUp()

        e_meta_0 = self.makeMeta(True, 0, 35)
        e_meta_1 = self.makeMeta(False, 1, 47)

        self.tb.run()

        self.assertTrue(pmt.equal(self.debug.get_message(0), pmt.cons(e_meta_0, pmt.init_u8vector(16, e_pdu_data_0))))
        self.assertTrue(pmt.equal(self.debug.get_message(1), pmt.cons(e_meta_1, pmt.init_u8vector(16, e_pdu_data_1))))

    # negative tests
    def test_004_t(self):
        data = [1, 0, 1, 1, 0, 1, 0, 0] * 10
        self.source = blocks.vector_source_b(data, False)
        # access code cannot be empty in permissive mode
        try:
            self.cut = pdu_utils.access_code_to_pdu('','', 16, 0, pdu_utils.SYNC_KEEP, pdu_utils.READ_PERMISSIVE)
            self.connectUp()
            self.tb.run()
            self.assertTrue(False)
        except (RuntimeError):
            print("GOT THIS FAR")
            self.assertTrue(True)
        # sum of lengths of syncwords cannot exceed burst length
        try:
            self.cut = pdu_utils.access_code_to_pdu('000101001010101','0101011100111', 8, 0, pdu_utils.SYNC_KEEP, pdu_utils.READ_PERMISSIVE)
            self.connectUp()
            self.tb.run()
            self.assertTrue(False)
        except (RuntimeError):
            self.assertTrue(True)
        # syncword strings must be valid
        try:
            self.cut = pdu_utils.access_code_to_pdu('7','', 8, 0, pdu_utils.SYNC_KEEP, pdu_utils.READ_PERMISSIVE)
            self.connectUp()
            self.tb.run()
            self.assertTrue(False)
        except (RuntimeError):
            self.assertTrue(True)
        try:
            self.cut = pdu_utils.access_code_to_pdu('1010','one zero one', 8, 0, pdu_utils.SYNC_KEEP, pdu_utils.READ_PERMISSIVE)
            self.connectUp()
            self.tb.run()
            self.assertTrue(False)
        except (RuntimeError):
            self.assertTrue(True)

if __name__ == '__main__':
    gr_unittest.run(qa_access_code_to_pdu)
