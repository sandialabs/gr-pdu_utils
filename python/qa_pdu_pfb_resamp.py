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

class qa_pdu_pfb_resamp (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_001_instantiation_fff (self):
      self.emitter = pdu_utils.message_emitter()
      self.resamp = pdu_utils.pdu_pfb_resamp_fff([1],1,1.0)
      self.debug = blocks.message_debug()
      self.tb.msg_connect((self.emitter, 'msg'), (self.resamp, 'pdu_in'))
      self.tb.msg_connect((self.resamp, 'pdu_out'), (self.debug, 'store'))

      self.tb.start()
      time.sleep(.5)
      self.tb.stop()
      self.tb.wait()

      self.assertTrue(True)

    def test_002_instantiation_ccf (self):
      self.emitter = pdu_utils.message_emitter()
      self.resamp = pdu_utils.pdu_pfb_resamp_ccf([1],1,1.0)
      self.debug = blocks.message_debug()
      self.tb.msg_connect((self.emitter, 'msg'), (self.resamp, 'pdu_in'))
      self.tb.msg_connect((self.resamp, 'pdu_out'), (self.debug, 'store'))

      self.tb.start()
      time.sleep(.5)
      self.tb.stop()
      self.tb.wait()

      self.assertTrue(True)

    def test_003_instantiation_fff (self):
      self.emitter = pdu_utils.message_emitter()
      self.resamp = pdu_utils.pdu_pfb_resamp_ccc([1],1,1.0)
      self.debug = blocks.message_debug()
      self.tb.msg_connect((self.emitter, 'msg'), (self.resamp, 'pdu_in'))
      self.tb.msg_connect((self.resamp, 'pdu_out'), (self.debug, 'store'))

      self.tb.start()
      time.sleep(.5)
      self.tb.stop()
      self.tb.wait()

      self.assertTrue(True)

if __name__ == '__main__':
    gr_unittest.run(qa_pdu_pfb_resamp)
