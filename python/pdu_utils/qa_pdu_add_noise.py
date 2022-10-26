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

class qa_pdu_add_noise (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()
        self.emitter = pdu_utils.message_emitter()
        self.add = pdu_utils.pdu_add_noise(0, 0, 1)
        self.debug = blocks.message_debug()
        self.tb.msg_connect((self.emitter, 'msg'), (self.add, 'pdu_in'))
        self.tb.msg_connect((self.add, 'pdu_out'), (self.debug, 'store'))

    def tearDown (self):
        self.tb = None

    def test_001_nochange (self):
        self.add.set_seed(123)
        in_data = range(8)
        expected_data = range(8)
        in_pdu = pmt.cons(pmt.make_dict(), pmt.init_u8vector(len(in_data), in_data))
        expected_pdu = pmt.cons(pmt.make_dict(), pmt.init_u8vector(len(expected_data), expected_data))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(pmt.intern("BAD INPUT"))
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        self.assertTrue(pmt.equal(self.debug.get_message(0), expected_pdu))

    def test_002_u8_scale_offset (self):
        self.add.set_scale(4)
        self.add.set_offset(3)
        self.add.set_seed(123)

        in_data = range(8)
        expected_data = range(3,32,4)
        in_pdu = pmt.cons(pmt.make_dict(), pmt.init_u8vector(len(in_data), in_data))
        expected_pdu = pmt.cons(pmt.make_dict(), pmt.init_u8vector(len(expected_data), expected_data))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(pmt.cons(pmt.intern("NONPDU"), pmt.intern("PAIR")))
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        self.assertTrue(pmt.equal(self.debug.get_message(0), expected_pdu))

    def test_003_f32noise (self):
        self.add.set_scale(4)
        self.add.set_offset(3)
        self.add.set_noise_level(0.1)
        self.add.set_seed(123)

        in_data = range(8)
        expected_data = [2.740996, 6.839901, 11.258297, 15.081026, 18.838703, 23.235038, 27.391647, 30.932546]
        in_pdu = pmt.cons(pmt.make_dict(), pmt.init_f32vector(len(in_data), in_data))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(pmt.intern("MALFORMED PDU"))
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()
        
        out_data = pmt.f32vector_elements(pmt.cdr(self.debug.get_message(0)))

        self.assertComplexTuplesAlmostEqual(out_data, expected_data, 3)

    def test_003_c32noise (self):
        self.add.set_scale(4)
        self.add.set_offset(3)
        self.add.set_noise_level(0.1)
        self.add.set_seed(123)

        in_data = range(8)
        expected_data = [(2.816856+2.886793j), (7.182643+3.057293j), (10.885946+3.166197j), (15.276935+2.952303j), (19.131071+3.236950j), (23.275022+3.002525j), (26.943286+2.933555j), (30.980100+3.243190j)]
        in_pdu = pmt.cons(pmt.make_dict(), pmt.init_c32vector(len(in_data), in_data))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(pmt.intern("MALFORMED PDU"))
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        out_data = pmt.c32vector_elements(pmt.cdr(self.debug.get_message(0)))

        self.assertComplexTuplesAlmostEqual(out_data, expected_data, 3)

    def test_004_c32noise_gaus (self):
        self.add.set_scale(4)
        self.add.set_offset(3)
        self.add.set_noise_level(0.1)
        self.add.set_noise_dist(pdu_utils.GAUSSIAN)
        self.add.set_seed(123)

        in_data = range(8)
        expected_data = [(2.844643+2.748667j),(7.105795+3.337258j),(11.271471+2.813702j),(14.992266+3.044907j),(19.103312+3.057148j),(23.000870+3.094649j),(26.533630+2.601930j),(31.216684+2.982270j)]

        in_pdu = pmt.cons(pmt.make_dict(), pmt.init_c32vector(len(in_data), in_data))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(pmt.intern("MALFORMED PDU"))
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        out_data = pmt.c32vector_elements(pmt.cdr(self.debug.get_message(0)))

        self.assertComplexTuplesAlmostEqual(out_data, expected_data, 3)

if __name__ == '__main__':
    gr_unittest.run(qa_pdu_add_noise)
