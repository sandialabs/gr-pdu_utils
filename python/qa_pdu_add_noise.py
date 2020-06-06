#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# <COPYRIGHT PLACEHOLDER>
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

        in_data = range(8)
        expected_data = [2.7966434955596924, 7.237407207489014, 11.077142715454102, 14.856366157531738, 18.887033462524414, 22.75377082824707, 26.903127670288086, 30.93477439880371]
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

        self.assertComplexTuplesAlmostEqual(out_data, expected_data)

    def test_003_c32noise (self):
        self.add.set_scale(4)
        self.add.set_offset(3)
        self.add.set_noise_level(0.1)

        in_data = range(8)
        expected_data = ((2.8562052249908447+3.167872428894043j), (7.054548263549805+2.898435354232788j), (10.920120239257812+2.8258891105651855j), (14.931501388549805+2.9538779258728027j), (18.730989456176758+2.91664981842041j), (22.851964950561523+2.7894411087036133j), (26.93660545349121+3.223921537399292j), (31.102128982543945+3.0600478649139404j))
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

        self.assertComplexTuplesAlmostEqual(out_data, expected_data)

    def test_004_c32noise_gaus (self):
        self.add.set_scale(4)
        self.add.set_offset(3)
        self.add.set_noise_level(0.1)
        self.add.set_noise_dist(pdu_utils.GAUSSIAN)

        in_data = range(8)
        expected_data = ((3.213324785232544+2.8172717094421387j), (6.527881145477295+3.2535653114318848j), (10.679037094116211+2.8527472019195557j), (14.64943790435791+2.4793572425842285j), (18.989013671875+2.964545965194702j), (22.857892990112305+2.9000906944274902j), (27.240381240844727+2.931946039199829j), (31.267457962036133+3.4548940658569336j))
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

        self.assertComplexTuplesAlmostEqual(out_data, expected_data)



if __name__ == '__main__':
    gr_unittest.run(qa_pdu_add_noise, "qa_pdu_add_noise.xml")
