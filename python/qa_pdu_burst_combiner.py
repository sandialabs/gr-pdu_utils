#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright 2018 National Technology & Engineering Solutions of Sandia, LLC (NTESS). 
# Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government retains 
# certain rights in this software.
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
from gnuradio.filter import firdes
import pmt
import time

class qa_pdu_burst_combiner (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()
        self.emitter = pdu_utils.message_emitter()
        self.combiner = pdu_utils.pdu_burst_combiner()
        self.debug = blocks.message_debug()
        self.tb.msg_connect((self.emitter, 'msg'), (self.combiner, 'pdu_in'))
        self.tb.msg_connect((self.combiner, 'pdu_out'), (self.debug, 'store'))

    def tearDown (self):
        self.tb = None

    def test_001_three (self):
        in_data1 = [1+1j, 0-1j]
        in_data2 = [1, 0]
        in_data3 = [1j, -1]
        expected_data = in_data1 + in_data2 + in_data3
        dict1 = pmt.dict_add(pmt.make_dict(), pmt.intern("burst_index"), pmt.cons(pmt.from_uint64(1), pmt.from_uint64(3)))
        dict2 = pmt.dict_add(pmt.make_dict(), pmt.intern("burst_index"), pmt.cons(pmt.from_uint64(2), pmt.from_uint64(3)))
        dict3 = pmt.dict_add(pmt.make_dict(), pmt.intern("burst_index"), pmt.cons(pmt.from_uint64(3), pmt.from_uint64(3)))
        in_pdu1 = pmt.cons(dict1, pmt.init_c32vector(len(in_data1), in_data1))
        in_pdu2 = pmt.cons(dict2, pmt.init_c32vector(len(in_data2), in_data2))
        in_pdu3 = pmt.cons(dict3, pmt.init_c32vector(len(in_data3), in_data3))
        expected_pdu = pmt.cons(pmt.make_dict(), pmt.init_c32vector(len(expected_data), expected_data))

        self.tb.start()
        time.sleep(.001)
        # handle non-pair input
        self.emitter.emit(pmt.intern("MALFORMED PDU"))
        time.sleep(.001)
        # handle malformed pair
        self.emitter.emit(pmt.cons(pmt.intern("NON-PDU"), pmt.intern("PAIR")))
        time.sleep(.001)
        # handle out of order PDU
        self.emitter.emit(in_pdu3)
        time.sleep(.001)
        self.emitter.emit(in_pdu1)
        time.sleep(.001)
        self.emitter.emit(in_pdu2)
        time.sleep(.001)
        self.emitter.emit(in_pdu3)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        out_data = pmt.c32vector_elements(pmt.cdr(self.debug.get_message(0)))
        self.assertComplexTuplesAlmostEqual(out_data, expected_data)

    def test_002_oneshot (self):
        in_data1 = [1+1j, 0-1j]
        expected_data = in_data1
        dict1 = pmt.dict_add(pmt.make_dict(), pmt.intern("burst_index"), pmt.cons(pmt.from_uint64(1), pmt.from_uint64(1)))
        in_pdu1 = pmt.cons(dict1, pmt.init_c32vector(len(in_data1), in_data1))
        expected_pdu = pmt.cons(pmt.make_dict(), pmt.init_c32vector(len(expected_data), expected_data))

        self.tb.start()
        time.sleep(.001)
        # handle non-pair input
        self.emitter.emit(pmt.intern("MALFORMED PDU"))
        time.sleep(.001)
        # handle malformed pair
        self.emitter.emit(pmt.cons(pmt.intern("NON-PDU"), pmt.intern("PAIR")))
        time.sleep(.001)
        self.emitter.emit(in_pdu1)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        out_data = pmt.c32vector_elements(pmt.cdr(self.debug.get_message(0)))
        self.assertComplexTuplesAlmostEqual(out_data, expected_data)


if __name__ == '__main__':
    gr_unittest.run(qa_pdu_burst_combiner)
