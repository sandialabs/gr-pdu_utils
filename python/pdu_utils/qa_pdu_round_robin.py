#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright 2018-2021 National Technology & Engineering Solutions of Sandia, LLC
# (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
# retains certain rights in this software.
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

from gnuradio import gr, gr_unittest
from gnuradio import blocks
try:
  from gnuradio import pdu_utils
except ImportError:
    import os
    import sys
    dirname, filename = os.path.split(os.path.abspath(__file__))
    sys.path.append(os.path.join(dirname, "bindings"))
    from gnuradio import pdu_utils

import pmt
import time

class qa_pdu_round_robin (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()
        self.emitter = pdu_utils.message_emitter(pmt.PMT_NIL)
        self.num_paths = 3
        self.pdu_round_robin = pdu_utils.pdu_round_robin(self.num_paths)
        self.debug = []
        for i in range(self.num_paths):
          self.debug.append(blocks.message_debug())

        self.tb.msg_connect((self.emitter, "msg"), (self.pdu_round_robin, "pdu_in"))
        for i in range(self.num_paths):
          self.tb.msg_connect((self.pdu_round_robin, "pdu_out_{}".format(i)), (self.debug[i], "store"))
        

    def tearDown (self):
        self.tb = None

    def test_001_t (self):
        # set up fg
        self.tb.start()
        # Create N messages with an id field
        p = pmt.make_dict()
        num_msgs = 20
        for i in range(num_msgs):
          pc = pmt.dict_add(p,pmt.intern("id"), pmt.from_uint64(i))
          self.emitter.emit(pc)
        
        # Sleep for a little bit to let the messages finish propagating
        time.sleep(.05)
        self.tb.stop()
        self.tb.wait()
        msg_count = 0
        for i in range(self.num_paths):
          target = i
          msg_count += self.debug[i].num_messages()
          for m in range(self.debug[i].num_messages()):
            msg = self.debug[i].get_message(m)
            msg_id = pmt.to_uint64(pmt.dict_ref(msg, pmt.intern("id"), pmt.PMT_NIL))
            assert(msg_id == target and msg_id < num_msgs)
            target += self.num_paths
        
        assert(msg_count == num_msgs)


if __name__ == '__main__':
    gr_unittest.run(qa_pdu_round_robin, "qa_pdu_round_robin.xml")
