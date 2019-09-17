#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 
# Copyright 2019 gr-pdu_utils author.
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

class qa_pdu_complex_to_mag2 (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_simple (self):
        self.emitter = pdu_utils.message_emitter()
        self.ctm2 = pdu_utils.pdu_complex_to_mag2()
        self.debug = blocks.message_debug()
        self.tb.msg_connect((self.emitter, 'msg'), (self.ctm2, 'cpdus'))
        self.tb.msg_connect((self.ctm2, 'fpdus'), (self.debug, 'store'))

        # gnuradio uses single-precision floats by default
        i_vec = pmt.init_c32vector(3, [3+4j, 1+0j, 0+1j])
        e_vec = pmt.init_f32vector(3, [25, 1, 1])

        in_pdu = pmt.cons(pmt.make_dict(), i_vec)
        e_pdu = pmt.cons(pmt.make_dict(), e_vec)

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        print "test ctm2:"
        print "pdu expected: " + repr(pmt.car(e_pdu))
        print "pdu got:      " + repr(pmt.car(self.debug.get_message(0)))
        print "data expected: " + repr(pmt.to_python(pmt.cdr(e_pdu)))
        print "data got:      " + repr(pmt.to_python(pmt.cdr(self.debug.get_message(0))))
        print

        self.assertTrue(pmt.equal(self.debug.get_message(0), e_pdu))


if __name__ == '__main__':
    gr_unittest.run(qa_pdu_complex_to_mag2, "qa_pdu_complex_to_mag2.xml")
