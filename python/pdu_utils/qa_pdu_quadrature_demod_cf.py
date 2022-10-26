#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright 2018-2021 National Technology & Engineering Solutions of Sandia, LLC
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

import pmt
import numpy as np
import time
from math import pi as PI

class qa_pdu_quadrature_demod_cf(gr_unittest.TestCase):

    def setUp(self):
        self.tb = gr.top_block()

    def tearDown(self):
        self.tb = None

    def test_001_t(self):
        self.emitter = pdu_utils.message_emitter()
        self.qd = pdu_utils.pdu_quadrature_demod_cf(1)
        self.debug = blocks.message_debug()

        self.tb.msg_connect((self.emitter, 'msg'), (self.qd, 'cpdus'))
        self.tb.msg_connect((self.qd, 'fpdus'), (self.debug, 'store'))

        in_data1 = [1., 1.j, -1., -1.j, 1.]
        in_data2 = [7.+7.j, -7.+7.j, -7.-7.j, 7.-7.j, 7.+7.j]
        expected_data = [PI/2, PI/2, PI/2, PI/2]
        in_pdu1 = pmt.cons(pmt.make_dict(), pmt.init_c32vector(len(in_data1), in_data1))
        in_pdu2 = pmt.cons(pmt.make_dict(), pmt.init_c32vector(len(in_data2), in_data2))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_pdu1)
        time.sleep(.01)
        self.emitter.emit(in_pdu2)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        out_data = pmt.f32vector_elements(pmt.cdr(self.debug.get_message(0)))
        self.assertFloatTuplesAlmostEqual(out_data, expected_data, 4)
        out_data = pmt.f32vector_elements(pmt.cdr(self.debug.get_message(1)))
        self.assertFloatTuplesAlmostEqual(out_data, expected_data, 4)


if __name__ == '__main__':
    gr_unittest.run(qa_pdu_quadrature_demod_cf)
