#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright 2018 National Technology & Engineering Solutions of Sandia, LLC (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government retains certain rights in this software.
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

import pdu_utils_swig as pdu_utils
from gnuradio import gr, gr_unittest
from gnuradio import blocks
import pmt
import time

class qa_message_gate (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_001_5x_pass (self):
        strobe = blocks.message_strobe(pmt.PMT_NIL, 25)
        ctr = pdu_utils.message_counter(pmt.intern("counter"))
        self.tb.msg_connect((strobe, 'strobe'), (ctr, 'msg'))

        self.tb.start()
        time.sleep(.135)
        self.tb.stop()
        self.tb.wait()

        self.assertEquals(5, ctr.get_ctr())


    def test_002_rst_3x_pass (self):
        strobe = blocks.message_strobe(pmt.PMT_NIL, 25)
        ctr = pdu_utils.message_counter(pmt.intern("counter"))
        self.tb.msg_connect((strobe, 'strobe'), (ctr, 'msg'))

        self.tb.start()
        time.sleep(.06)
        ctr.reset()
        time.sleep(.075)
        self.tb.stop()
        self.tb.wait()

        self.assertEquals(3, ctr.get_ctr())


if __name__ == '__main__':
    gr_unittest.run(qa_message_gate, "qa_message_gate.xml")
