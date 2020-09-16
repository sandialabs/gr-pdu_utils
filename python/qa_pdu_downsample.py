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

from gnuradio import gr, gr_unittest
from gnuradio import blocks
import pdu_utils_swig as pdu_utils
import pmt
import time


class qa_pdu_downsample (gr_unittest.TestCase):

    def setUp(self):
        self.tb = gr.top_block()
        self.emitter = pdu_utils.message_emitter()
        self.debug = blocks.message_debug()

    def connectUp(self):
        self.tb.msg_connect((self.emitter, 'msg'), (self.down, 'pdu_in'))
        self.tb.msg_connect((self.down, 'pdu_out'), (self.debug, 'store'))

    def tearDown(self):
        self.tb = None

    def test_uint8(self):
        self.down = pdu_utils.pdu_downsample(2, 1)
        self.connectUp()

        i_vec = pmt.init_u8vector(6, [0, 1, 2, 3, 4, 5])
        e_vec = pmt.init_u8vector(3, [1, 3, 5])
        in_pdu = pmt.cons(pmt.make_dict(), i_vec)
        e_pdu = pmt.cons(pmt.make_dict(), e_vec)

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        print("test uint8:")
        print("pdu expected: " + repr(pmt.car(e_pdu)))
        print("pdu got:      " + repr(pmt.car(self.debug.get_message(0))))
        print("data expected: " + repr(pmt.u8vector_elements(pmt.cdr(e_pdu))))
        print("data got:      " + repr(pmt.u8vector_elements(pmt.cdr(self.debug.get_message(0)))))
        print

        self.assertTrue(pmt.equal(self.debug.get_message(0), e_pdu))

    def test_longer(self):
        self.down = pdu_utils.pdu_downsample(2, 1)
        self.connectUp()

        i_vec = pmt.init_u8vector(7, [0, 1, 2, 3, 4, 5, 6])
        e_vec = pmt.init_u8vector(3, [1, 3, 5])
        in_pdu = pmt.cons(pmt.make_dict(), i_vec)
        e_pdu = pmt.cons(pmt.make_dict(), e_vec)

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        print("test long:")
        print("pdu expected: " + repr(pmt.car(e_pdu)))
        print("pdu got:      " + repr(pmt.car(self.debug.get_message(0))))
        print("data expected: " + repr(pmt.u8vector_elements(pmt.cdr(e_pdu))))
        print("data got:      " + repr(pmt.u8vector_elements(pmt.cdr(self.debug.get_message(0)))))
        print

        self.assertTrue(pmt.equal(self.debug.get_message(0), e_pdu))

    def test_float(self):
        self.down = pdu_utils.pdu_downsample(3, 0)
        self.connectUp()

        i_vec = pmt.init_f32vector(9, [0, 1, 2, 3.3, 4, 5, 6, 7, 8])
        e_vec = pmt.init_f32vector(3, [0, 3.3, 6])
        in_pdu = pmt.cons(pmt.make_dict(), i_vec)
        e_pdu = pmt.cons(pmt.make_dict(), e_vec)

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        print("test float:")
        print("pdu expected: " + repr(pmt.car(e_pdu)))
        print("pdu got:      " + repr(pmt.car(self.debug.get_message(0))))
        print("data expected: " + repr(pmt.f32vector_elements(pmt.cdr(e_pdu))))
        print("data got:      " + repr(pmt.f32vector_elements(pmt.cdr(self.debug.get_message(0)))))
        print

        self.assertTrue(pmt.equal(self.debug.get_message(0), e_pdu))

    def test_c32vector(self):
        '''
        Note: this is a negative test. Modules doesn't support c32vector, so this is testing that
        '''
        self.down = pdu_utils.pdu_downsample(3, 0)
        self.connectUp()

        i_vec = pmt.init_c32vector(3, [3 + 4j, 1 + 0j, 0 + 1j])
        in_pdu = pmt.cons(pmt.make_dict(), i_vec)

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        self.assertEqual(0, self.debug.num_messages())


if __name__ == '__main__':
    gr_unittest.run(qa_pdu_downsample)
