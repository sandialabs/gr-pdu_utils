#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright 2018, 2019, 2020 National Technology & Engineering Solutions of Sandia, LLC
# (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
# retains certain rights in this software.
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

from gnuradio import gr, gr_unittest
from gnuradio import blocks
import pdu_utils_swig as pdu_utils
import time
import pmt

class qa_sandia_message_debug(gr_unittest.TestCase):

    def setUp(self):
        self.tb = gr.top_block()

    def tearDown(self):
        self.tb = None

    def test_001(self):
      self.emitter = pdu_utils.message_emitter()
      self.debug = pdu_utils.sandia_message_debug()

      # make connections
      self.tb.msg_connect((self.emitter,'msg'),(self.debug,'print'))

      self.tb.start()
      self.emitter.emit(pmt.intern("TEST"))
      time.sleep(.1)
      self.tb.stop()
      self.tb.wait()

      self.assertTrue(True)

if __name__ == '__main__':
    gr_unittest.run(qa_sandia_message_debug)
