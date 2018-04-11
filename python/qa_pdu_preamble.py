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


class qa_pdu_preamble (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_001_basic_nrz_test (self):
        emitter = pdu_utils.message_emitter()
        preamble = pdu_utils.pdu_preamble([0,1,0,1],[],2,3,True)
        debug = blocks.message_debug()
        self.tb.msg_connect((emitter, 'msg'), (preamble, 'pdu_in'))
        self.tb.msg_connect((preamble, 'pdu_out'), (debug, 'store'))

        input_data = pmt.init_u8vector(8, [1,0,1,1,0,0,1,0])
        input_dict = pmt.dict_add(pmt.make_dict(), pmt.intern("KEY"), pmt.intern("VALUE"))
        expected_data = pmt.init_f32vector((8+4+0)*2 + 3*2, [0,0,0,-1,-1,1,1,-1,-1,1,1,1,1,-1,-1,1,1,1,1,-1,-1,-1,-1,1,1,-1,-1,0,0,0])
        self.tb.start()
        time.sleep(.001)
        emitter.emit(pmt.intern("MALFORMED PDU"))
        time.sleep(.001)
        emitter.emit(pmt.cons(input_dict, input_data))
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()
        output_data = pmt.cdr(debug.get_message(0))

        self.assertEquals(1, debug.num_messages())
        self.assertTrue((pmt.f32vector_elements(expected_data) == pmt.f32vector_elements(output_data)))

"""
Tests to add:
 - test callbacks
 - test Tail
 - test error conditions
 - test alternate inputs
"""

if __name__ == '__main__':
    gr_unittest.run(qa_pdu_preamble, "qa_pdu_preamble.xml")
