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


class qa_message_counter (gr_unittest.TestCase):

    def setUp(self):
        self.tb = gr.top_block()
        self.emitter = pdu_utils.message_emitter()
        self.ctr = pdu_utils.message_counter("counter")
        self.tb.msg_connect((self.emitter, 'msg'), (self.ctr, 'msg'))

    def tearDown(self):
        self.tb = None

    def test_001_5x_pass(self):

        self.tb.start()
        for x in range(5):
            time.sleep(.001)
            self.emitter.emit()
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        self.assertEqual(5, self.ctr.get_ctr())

    def test_002_rst_3x_pass(self):

        self.tb.start()
        for x in range(5):
            time.sleep(.001)
            self.emitter.emit()
        time.sleep(.1)
        self.assertEqual(5, self.ctr.get_ctr())

        self.ctr.reset()
        self.assertEqual(0, self.ctr.get_ctr())

        for x in range(3):
            time.sleep(.001)
            self.emitter.emit()
        time.sleep(.1)

        self.tb.stop()
        self.tb.wait()

        self.assertEqual(3, self.ctr.get_ctr())

    def test_003_get_name(self):
        '''
        tests get_name function
        '''

        print('test_003_get_name() - name is ', self.ctr.get_name())
        self.assertEqual('counter', self.ctr.get_name())

        self.ctr = pdu_utils.message_counter("BoBo")
        self.assertEqual('BoBo', self.ctr.get_name())


if __name__ == '__main__':
    gr_unittest.run(qa_message_counter, "qa_message_counter.xml")
