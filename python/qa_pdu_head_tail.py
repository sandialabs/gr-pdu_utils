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


class qa_pdu_head_tail (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()
        self.emitter = pdu_utils.message_emitter()
        self.ht = pdu_utils.pdu_head_tail(pdu_utils.INPUTTYPE_UNPACKED_BYTE, 16, 1)
        self.d_head = blocks.message_debug()
        self.d_tail = blocks.message_debug()
        self.tb.msg_connect((self.emitter, 'msg'), (self.ht, 'pdu_in'))
        self.tb.msg_connect((self.ht, 'head'), (self.d_head, 'store'))
        self.tb.msg_connect((self.ht, 'tail'), (self.d_tail, 'store'))


    def tearDown (self):
        self.tb = None

    def test_001_smaller (self):
        self.ht.set_length(16)
        in_data = [1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1]
        in_pdu = pmt.cons(pmt.make_dict(), pmt.init_u8vector(len(in_data), in_data))
        expected_head = [1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1]
        e_head_pdu = pmt.cons(pmt.make_dict(), pmt.init_u8vector(len(expected_head), expected_head))
        expected_tail = [1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1]
        e_tail_pdu = pmt.cons(pmt.make_dict(), pmt.init_u8vector(len(expected_tail), expected_tail))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(pmt.intern("MALFORMED PDU"))
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        self.assertTrue(pmt.equal(self.d_head.get_message(0), e_head_pdu))
        self.assertTrue(pmt.equal(self.d_tail.get_message(0), e_tail_pdu))


    def test_002_larger (self):
        self.ht.set_length(32)
        in_data = [1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1]
        in_pdu = pmt.cons(pmt.make_dict(), pmt.init_u8vector(len(in_data), in_data))
        expected_head = [1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0]
        e_head_pdu = pmt.cons(pmt.make_dict(), pmt.init_u8vector(len(expected_head), expected_head))
        expected_tail = [0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1]
        e_tail_pdu = pmt.cons(pmt.make_dict(), pmt.init_u8vector(len(expected_tail), expected_tail))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(pmt.intern("MALFORMED PDU"))
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        self.assertTrue(pmt.equal(self.d_head.get_message(0), e_head_pdu))
        self.assertTrue(pmt.equal(self.d_tail.get_message(0), e_tail_pdu))


    def test_003_hist (self):
        self.ht.set_length(4)
        self.ht.set_histsize(4)
        in_data0 = [0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0]
        in_data1 = [0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0]
        in_data2 = [0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0]
        in_data3 = [0,41, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0]
        in_data4 = [1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1]
        expected_head = [0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0]
        e_head_pdu = pmt.cons(pmt.make_dict(), pmt.init_u8vector(len(expected_head), expected_head))
        expected_tail = [1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1]
        e_tail_pdu = pmt.cons(pmt.make_dict(), pmt.init_u8vector(len(expected_tail), expected_tail))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(pmt.cons(pmt.make_dict(), pmt.init_u8vector(len(in_data0), in_data0)))
        time.sleep(.001)
        self.emitter.emit(pmt.cons(pmt.make_dict(), pmt.init_u8vector(len(in_data1), in_data1)))
        time.sleep(.001)
        self.emitter.emit(pmt.cons(pmt.make_dict(), pmt.init_u8vector(len(in_data2), in_data2)))
        time.sleep(.001)
        self.emitter.emit(pmt.cons(pmt.make_dict(), pmt.init_u8vector(len(in_data3), in_data3)))
        time.sleep(.001)
        self.emitter.emit(pmt.cons(pmt.make_dict(), pmt.init_u8vector(len(in_data4), in_data4)))
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        self.assertTrue(pmt.equal(self.d_head.get_message(4), e_head_pdu))
        self.assertTrue(pmt.equal(self.d_tail.get_message(4), e_tail_pdu))


if __name__ == '__main__':
    gr_unittest.run(qa_pdu_head_tail)
