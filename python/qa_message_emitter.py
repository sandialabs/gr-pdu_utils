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

class qa_message_emitter (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_001_emit3 (self):
        emitter = pdu_utils.message_emitter()
        debug = blocks.message_debug()
        self.tb.msg_connect((emitter, 'msg'), (debug, 'store'))

        self.tb.start()
        time.sleep(.001)
        emitter.emit()
        time.sleep(.001)
        emitter.emit()
        time.sleep(.001)
        emitter.emit()
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        self.assertEquals(3, debug.num_messages())


    def test_002_emit2 (self):
        emitter = pdu_utils.message_emitter()
        debug = blocks.message_debug()
        msg = pmt.cons(pmt.intern("TEST MESSAGE"), pmt.init_u8vector(4,range(4)))
        self.tb.msg_connect((emitter, 'msg'), (debug, 'store'))

        self.tb.start()
        time.sleep(.001)
        emitter.emit()
        time.sleep(.001)
        emitter.emit(msg)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        print emitter.get_n_msgs()
        self.assertEquals(2, debug.num_messages())
        self.assertTrue(pmt.eqv(pmt.PMT_NIL, debug.get_message(0)))
        self.assertTrue(pmt.eqv(msg, debug.get_message(1)))


if __name__ == '__main__':
    gr_unittest.run(qa_message_emitter, "qa_message_emitter.xml")
