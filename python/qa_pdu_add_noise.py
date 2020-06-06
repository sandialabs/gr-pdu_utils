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
import pmt
import time

class qa_pdu_add_noise (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()
        self.emitter = pdu_utils.message_emitter()
        self.add = pdu_utils.pdu_add_noise(0, 0, 1)
        self.debug = blocks.message_debug()
        self.tb.msg_connect((self.emitter, 'msg'), (self.add, 'pdu_in'))
        self.tb.msg_connect((self.add, 'pdu_out'), (self.debug, 'store'))

    def tearDown (self):
        self.tb = None

    def test_001_nochange (self):
        self.add.set_seed(123)
        in_data = range(8)
        expected_data = range(8)
        in_pdu = pmt.cons(pmt.make_dict(), pmt.init_u8vector(len(in_data), in_data))
        expected_pdu = pmt.cons(pmt.make_dict(), pmt.init_u8vector(len(expected_data), expected_data))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(pmt.intern("BAD INPUT"))
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        self.assertTrue(pmt.equal(self.debug.get_message(0), expected_pdu))

    def test_002_u8_scale_offset (self):
        self.add.set_scale(4)
        self.add.set_offset(3)
        self.add.set_seed(123)

        in_data = range(8)
        expected_data = range(3,32,4)
        in_pdu = pmt.cons(pmt.make_dict(), pmt.init_u8vector(len(in_data), in_data))
        expected_pdu = pmt.cons(pmt.make_dict(), pmt.init_u8vector(len(expected_data), expected_data))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(pmt.cons(pmt.intern("NONPDU"), pmt.intern("PAIR")))
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        self.assertTrue(pmt.equal(self.debug.get_message(0), expected_pdu))

    def test_003_f32noise (self):
        self.add.set_scale(4)
        self.add.set_offset(3)
        self.add.set_noise_level(0.1)
        self.add.set_seed(123)

        in_data = range(8)
        expected_data = [3.1571753, 7.1703644, 10.828911, 14.942777, 18.78148, 23.152708, 27.041052, 31.17532]
        in_pdu = pmt.cons(pmt.make_dict(), pmt.init_f32vector(len(in_data), in_data))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(pmt.intern("MALFORMED PDU"))
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()
        
        out_data = pmt.f32vector_elements(pmt.cdr(self.debug.get_message(0)))

        self.assertComplexTuplesAlmostEqual(out_data, expected_data, 3)

    def test_003_c32noise (self):
        self.add.set_scale(4)
        self.add.set_offset(3)
        self.add.set_noise_level(0.1)
        self.add.set_seed(123)

        in_data = range(8)
        expected_data = [(3.1111398+3.1204658j), (6.879022+2.959537j), (10.845484+3.1079807j), (15.029028+3.12397j), (19.124151+2.994976j), (22.956503+3.1584077j), (27.271961+2.9496112j), (31.104555+3.0450819j)]
        in_pdu = pmt.cons(pmt.make_dict(), pmt.init_c32vector(len(in_data), in_data))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(pmt.intern("MALFORMED PDU"))
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        out_data = pmt.c32vector_elements(pmt.cdr(self.debug.get_message(0)))

        self.assertComplexTuplesAlmostEqual(out_data, expected_data, 3)

    def test_004_c32noise_gaus (self):
        self.add.set_scale(4)
        self.add.set_offset(3)
        self.add.set_noise_level(0.1)
        self.add.set_noise_dist(pdu_utils.GAUSSIAN)
        self.add.set_seed(123)

        in_data = range(8)
        expected_data = [(3.3071127+3.283337j), (6.8398867+2.5212853j), (11.206407+2.7046404j), (15.492073+3.1152205j), (18.979256+3.5126355j), (23.402103+2.8895853j), (26.984592+3.0831635j), (31.213652+3.4955065j)]
        in_pdu = pmt.cons(pmt.make_dict(), pmt.init_c32vector(len(in_data), in_data))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(pmt.intern("MALFORMED PDU"))
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        out_data = pmt.c32vector_elements(pmt.cdr(self.debug.get_message(0)))

        self.assertComplexTuplesAlmostEqual(out_data, expected_data, 3)

if __name__ == '__main__':
    gr_unittest.run(qa_pdu_add_noise)
