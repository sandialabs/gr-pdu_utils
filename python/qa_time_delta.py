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

from gnuradio import gr, gr_unittest
from gnuradio import blocks
import pdu_utils_swig as pdu_utils
import pmt
import numpy
import time

class qa_time_delta (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

        self.emitter = pdu_utils.message_emitter(pmt.PMT_NIL)
        self.time = pdu_utils.time_delta("TIME CHECKER")
        self.ctr = pdu_utils.message_counter(pmt.intern("counter"))

        self.tb.msg_connect((self.emitter, 'msg'), (self.time, 'pdu_in'))
        self.tb.msg_connect((self.emitter, 'msg'), (self.ctr, 'msg'))


    def tearDown (self):
        self.tb = None

    def test_001_t (self):
        in_data = [0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1]
        meta = pmt.dict_add(pmt.make_dict(), pmt.intern('wall_clock_time'), pmt.from_double(time.time()-10))
        in_pdu = pmt.cons(meta, pmt.init_c32vector(len(in_data), in_data))
        self.ctr.reset()

        # set up fg
        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(pmt.intern("MALFORMED PDU"))
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()
        # check data

        self.assertEquals(2, self.ctr.get_ctr())


if __name__ == '__main__':
    gr_unittest.run(qa_time_delta)
