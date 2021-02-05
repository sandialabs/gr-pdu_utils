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
    import pdu_utils
except ImportError:
    import os
    import sys
    dirname, filename = os.path.split(os.path.abspath(__file__))
    sys.path.append(os.path.join(dirname, "bindings"))
    import pdu_utils
from gnuradio import gr, gr_unittest
from gnuradio import blocks
import numpy as np
import time
import pmt

class qa_pdu_rotate(gr_unittest.TestCase):

    def rotate(self, data, phase_inc):
      return [d*r for d,r in zip(data, np.exp(1j*  np.arange(0, phase_inc*len(data), phase_inc, dtype=np.complex64)     ))]

    def setUp(self):
        self.tb = gr.top_block()
        self.emitter = pdu_utils.message_emitter()
        self.debug = blocks.message_debug()

    def connectUp(self):
        self.tb.msg_connect((self.emitter, 'msg'), (self.dut, 'pdu_in'))
        self.tb.msg_connect((self.dut, 'pdu_out'), (self.debug, 'store'))

    def tearDown(self):
        self.tb = None

    def test_normal_use(self):
        phase_inc = np.pi/4
        self.dut = pdu_utils.pdu_rotate(phase_inc)
        self.connectUp()

        data = [x+x*1j for x in range(16)]
        rot_data = self.rotate(data, -1*phase_inc)
        in_pdu = pmt.cons(pmt.make_dict(), pmt.init_c32vector(len(rot_data), rot_data))
        e_vec = pmt.init_c32vector(len(data), data)

        self.tb.start()
        time.sleep(.01)
        self.emitter.emit(in_pdu)
        time.sleep(.1)
        self.tb.stop()
        self.tb.wait()

        #print("test_001:")
        #print(f"expected: {data}")
        #print(f"got: {self.debug.get_message(0)}")

        rcv = pmt.c32vector_elements(pmt.cdr(self.debug.get_message(0)))
        self.assertComplexTuplesAlmostEqual(rcv, data, 2)

    def test_metadata_phase_set(self):
        phase_inc = np.pi/4
        self.dut = pdu_utils.pdu_rotate(phase_inc)
        self.connectUp()

        data = [x+x*1j for x in range(16)]
        rot_data = self.rotate(data, -2*phase_inc)
        meta = pmt.make_dict()
        meta = pmt.dict_add(meta, pmt.intern("phase_inc"), pmt.from_double(phase_inc*2))
        in_pdu = pmt.cons(meta, pmt.init_c32vector(len(rot_data), rot_data))
        e_vec = pmt.init_c32vector(len(data), data)

        self.tb.start()
        time.sleep(.01)
        self.emitter.emit(in_pdu)
        time.sleep(.1)
        self.tb.stop()
        self.tb.wait()

        #print("test_metadata_phase_inc:")
        #print(f"expected: {data}")
        #print(f"got: {self.debug.get_message(0)}")

        rcv = pmt.c32vector_elements(pmt.cdr(self.debug.get_message(0)))
        self.assertComplexTuplesAlmostEqual(rcv, data, 2)



if __name__ == '__main__':
    gr_unittest.run(qa_pdu_rotate)
