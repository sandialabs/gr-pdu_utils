#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 
# Copyright 2018 gr-pdu_utils author.
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

class qa_pdu_binary_tools (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_bit_flip (self):
        self.emitter = pdu_utils.message_emitter()
        #self.flip = pdu_utils.pdu_binary_tools(pdu_utils.pdu_binary_tools.BIT_FLIP)
        self.flip = pdu_utils.pdu_binary_tools(0)
        self.debug = blocks.message_debug()
        self.tb.msg_connect((self.emitter, 'msg'), (self.flip, 'pdu_in'))
        self.tb.msg_connect((self.flip, 'pdu_out'), (self.debug, 'store'))

        i_vec = pmt.init_u8vector(6, [1,0,0,1,0,1])
        e_vec = pmt.init_u8vector(6, [0,1,1,0,1,0])
        in_pdu = pmt.cons(pmt.make_dict(), i_vec)
        e_pdu = pmt.cons(pmt.make_dict(), e_vec)

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        print("test bit_flip:")
        print("pdu expected: " + repr(pmt.car(e_pdu)))
        print("pdu got:      " + repr(pmt.car(self.debug.get_message(0))))
        print("data expected: " + repr(pmt.u8vector_elements(pmt.cdr(e_pdu))))
        print("data got:      " + repr(pmt.u8vector_elements(pmt.cdr(self.debug.get_message(0)))))
        print(self.assertTrue(pmt.equal(self.debug.get_message(0), e_pdu)))

    def test_to_pam (self):
        self.emitter = pdu_utils.message_emitter()
        #self.flip = pdu_utils.pdu_binary_tools(pdu_utils.pdu_binary_tools.TO_PAM)
        self.flip = pdu_utils.pdu_binary_tools(1)
        self.debug = blocks.message_debug()
        self.tb.msg_connect((self.emitter, 'msg'), (self.flip, 'pdu_in'))
        self.tb.msg_connect((self.flip, 'pdu_out'), (self.debug, 'store'))

        i_vec = pmt.init_u8vector(6, [1,0,0,1,0,1])
        e_vec = pmt.init_f32vector(6, [1,-1,-1,1,-1,1])
        in_pdu = pmt.cons(pmt.make_dict(), i_vec)
        e_pdu = pmt.cons(pmt.make_dict(), e_vec)

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        print("test to_pam:")
        print("pdu expected: " + repr(pmt.car(e_pdu)))
        print("pdu got:      " + repr(pmt.car(self.debug.get_message(0))))
        print("data expected: " + repr(pmt.f32vector_elements(pmt.cdr(e_pdu))))
        print("data got:      " + repr(pmt.f32vector_elements(pmt.cdr(self.debug.get_message(0)))))
        print(self.assertTrue(pmt.equal(self.debug.get_message(0), e_pdu)))

    def test_from_pam (self):
        self.emitter = pdu_utils.message_emitter()
        #self.flip = pdu_utils.pdu_binary_tools(pdu_utils.pdu_binary_tools.FROM_PAM)
        self.flip = pdu_utils.pdu_binary_tools(2)
        self.debug = blocks.message_debug()
        self.tb.msg_connect((self.emitter, 'msg'), (self.flip, 'pdu_in'))
        self.tb.msg_connect((self.flip, 'pdu_out'), (self.debug, 'store'))

        i_vec = pmt.init_f32vector(6, [1,-1,-1,1,-1,1])
        e_vec = pmt.init_u8vector(6, [1,0,0,1,0,1])
        in_pdu = pmt.cons(pmt.make_dict(), i_vec)
        e_pdu = pmt.cons(pmt.make_dict(), e_vec)

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        print("test from_pam:")
        print("pdu expected: " + repr(pmt.car(e_pdu)))
        print("pdu got:      " + repr(pmt.car(self.debug.get_message(0))))
        print("data expected: " + repr(pmt.u8vector_elements(pmt.cdr(e_pdu))))
        print("data got:      " + repr(pmt.u8vector_elements(pmt.cdr(self.debug.get_message(0)))))
        print(self.assertTrue(pmt.equal(self.debug.get_message(0), e_pdu)))

    def test_slice (self):
        self.emitter = pdu_utils.message_emitter()
        #self.flip = pdu_utils.pdu_binary_tools(pdu_utils.pdu_binary_tools.SLICE)
        self.flip = pdu_utils.pdu_binary_tools(3)
        self.debug = blocks.message_debug()
        self.tb.msg_connect((self.emitter, 'msg'), (self.flip, 'pdu_in'))
        self.tb.msg_connect((self.flip, 'pdu_out'), (self.debug, 'store'))

        i_vec = pmt.init_f32vector(6, [2,-1,-2,.1,-.1,1000])
        e_vec = pmt.init_u8vector(6, [1,0,0,1,0,1])
        in_pdu = pmt.cons(pmt.make_dict(), i_vec)
        e_pdu = pmt.cons(pmt.make_dict(), e_vec)

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        print("test slice:")
        print("pdu expected: " + repr(pmt.car(e_pdu)))
        print("pdu got:      " + repr(pmt.car(self.debug.get_message(0))))
        print("data expected: " + repr(pmt.u8vector_elements(pmt.cdr(e_pdu))))
        print("data got:      " + repr(pmt.u8vector_elements(pmt.cdr(self.debug.get_message(0)))))
        print(self.assertTrue(pmt.equal(self.debug.get_message(0), e_pdu)))

    def test_endian (self):
        self.emitter = pdu_utils.message_emitter()
        #self.endi = pdu_utils.pdu_binary_tools(pdu_utils.pdu_binary_tools.ENDIAN_SWAP8)
        self.endi = pdu_utils.pdu_binary_tools(4)
        self.debug = blocks.message_debug()
        self.tb.msg_connect((self.emitter, 'msg'), (self.endi, 'pdu_in'))
        self.tb.msg_connect((self.endi, 'pdu_out'), (self.debug, 'store'))

        i_vec = pmt.init_u8vector(16, [1,0,1,0, 0,0,1,1,  1,1,0,1, 0,0,1,0])
        e_vec = pmt.init_u8vector(16, [1,1,0,0, 0,1,0,1,  0,1,0,0, 1,0,1,1])
        in_pdu = pmt.cons(pmt.make_dict(), i_vec)
        e_pdu = pmt.cons(pmt.make_dict(), e_vec)

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        print("test endian_swap8:")
        print("pdu expected: " + repr(pmt.car(e_pdu)))
        print("pdu got:      " + repr(pmt.car(self.debug.get_message(0))))
        print("data expected: " + repr(pmt.u8vector_elements(pmt.cdr(e_pdu))))
        print("data got:      " + repr(pmt.u8vector_elements(pmt.cdr(self.debug.get_message(0)))))
        print(self.assertTrue(pmt.equal(self.debug.get_message(0), e_pdu)))

if __name__ == '__main__':
    gr_unittest.run(qa_pdu_binary_tools)
