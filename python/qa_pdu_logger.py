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


class qa_pdu_logger (gr_unittest.TestCase):

    def setUp(self):
        self.tb = gr.top_block()
        self.emitter = pdu_utils.message_emitter()
        self.dut = pdu_utils.pdu_logger('/dev/null')
        self.tb.msg_connect((self.emitter, 'msg'), (self.dut, 'pdu_in'))

    def tearDown(self):
        self.tb = None

    def test_001_instantiate(self):

        in_data = [0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1]
        in_pdu = pmt.cons(pmt.make_dict(), pmt.init_u8vector(len(in_data), in_data))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()


if __name__ == '__main__':
    gr_unittest.run(qa_pdu_logger)
