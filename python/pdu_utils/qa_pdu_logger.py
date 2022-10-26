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
import os

QA_LOG_DIR = '/tmp/pdu_utils-pdu_logger-qa/'

class qa_pdu_logger (gr_unittest.TestCase):

    def setUp(self):
        self.tb = gr.top_block()
        self.emitter = pdu_utils.message_emitter()
        if not os.path.exists(QA_LOG_DIR):
            os.mkdir(QA_LOG_DIR)

    def tearDown(self):
        self.tb = None
        os.rmdir(QA_LOG_DIR)

    def test_001_simplelog(self):
        self.dut = pdu_utils.pdu_logger(QA_LOG_DIR+'raw-', 1)
        self.tb.msg_connect((self.emitter, 'msg'), (self.dut, 'pdu_in'))

        in_data_c32 = [4.1-2.4j, 0.-0.j, 1.+1.j, 1e-9-1e-9j]
        in_pdu_c32 = pmt.cons(pmt.make_dict(), pmt.init_c32vector(len(in_data_c32), in_data_c32))
        in_data_f32 = [4.1, -2.4, 0., -0., 1., 1., 1e-9, -1e-9]
        in_pdu_f32 = pmt.cons(pmt.make_dict(), pmt.init_f32vector(len(in_data_f32), in_data_f32))
        in_data_u8 = [1, 2, 3, 4, 5, 6, 7, 8]
        in_pdu_u8 = pmt.cons(pmt.make_dict(), pmt.init_u8vector(len(in_data_u8), in_data_u8))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_pdu_c32)
        time.sleep(.001)
        self.emitter.emit(in_pdu_f32)
        time.sleep(.001)
        self.emitter.emit(in_pdu_u8)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()
        
        #TODO check contents of PDUs
        os.remove(QA_LOG_DIR + 'raw-c32_0000.fc32')
        os.remove(QA_LOG_DIR + 'raw-f32_0001.f32')
        os.remove(QA_LOG_DIR + 'raw-u8_0002.u8')

    def test_002_badinput(self):
        self.dut = pdu_utils.pdu_logger(QA_LOG_DIR+'raw-')
        self.tb.msg_connect((self.emitter, 'msg'), (self.dut, 'pdu_in'))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(pmt.PMT_T)
        time.sleep(.001)
        self.emitter.emit(pmt.dict_add(pmt.make_dict(), pmt.intern('asdf'), pmt.intern('fdsa')))
        time.sleep(.001)
        self.emitter.emit(pmt.init_u8vector(4,[1,2,3,4]))
        time.sleep(.001)
        self.emitter.emit(pmt.cons(pmt.make_dict(), pmt.init_u16vector(4,[1,2,3,4])))
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()


if __name__ == '__main__':
    gr_unittest.run(qa_pdu_logger)
