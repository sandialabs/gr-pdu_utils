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

class qa_message_keep_1_in_n (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_001_send5_keep1 (self):
        emitter = pdu_utils.message_emitter()
        keep = pdu_utils.message_keep_1_in_n(3)
        debug = blocks.message_debug()
        self.tb.msg_connect((emitter, 'msg'), (keep, 'in'))
        self.tb.msg_connect((keep, 'out'), (debug, 'store'))

        self.tb.start()
        for x in range(5):
            time.sleep(.001)
            emitter.emit()
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        self.assertEquals(1, debug.num_messages())

    def test_001_send5_keep1 (self):
        emitter = pdu_utils.message_emitter()
        keep = pdu_utils.message_keep_1_in_n(3)
        debug = blocks.message_debug()
        self.tb.msg_connect((emitter, 'msg'), (keep, 'in'))
        self.tb.msg_connect((keep, 'out'), (debug, 'store'))

        self.tb.start()
        for x in range(5):
            time.sleep(.001)
            emitter.emit()
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        self.assertEquals(1, debug.num_messages())

    def test_002_send8_keep4 (self):
        emitter = pdu_utils.message_emitter()
        keep = pdu_utils.message_keep_1_in_n(2)
        debug = blocks.message_debug()
        self.tb.msg_connect((emitter, 'msg'), (keep, 'in'))
        self.tb.msg_connect((keep, 'out'), (debug, 'store'))

        self.tb.start()
        for x in range(8):
            time.sleep(.001)
            emitter.emit()
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        self.assertEquals(4, debug.num_messages())


if __name__ == '__main__':
    gr_unittest.run(qa_message_keep_1_in_n)
