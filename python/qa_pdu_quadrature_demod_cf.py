#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright 2020 gr-pdu_utils author.
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
