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

class qa_pdu_commutator (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()
        self.emitter = pdu_utils.message_emitter(pmt.PMT_NIL)
        self.num_paths = 3
        self.pdu_commutator = pdu_utils.pdu_commutator(self.num_paths)
        self.debug = []
        for i in range(self.num_paths):
          self.debug.append(blocks.message_debug())

        self.tb.msg_connect((self.emitter, "msg"), (self.pdu_commutator, "pdu_in"))
        for i in range(self.num_paths):
          self.tb.msg_connect((self.pdu_commutator, "pdu_out_{}".format(i)), (self.debug[i], "store"))
        

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
    gr_unittest.run(qa_pdu_commutator, "qa_pdu_commutator.xml")
