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

class qa_msg_drop_random (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()
        self.emitter = pdu_utils.message_emitter()
        self.drop = pdu_utils.msg_drop_random(0.5,1)
        self.debug = blocks.message_debug()
        self.tb.msg_connect((self.emitter, 'msg'), (self.drop, 'pdu_in'))
        self.tb.msg_connect((self.drop, 'pdu_out'), (self.debug, 'store'))
        self.in_msg = pmt.PMT_T
        self.n_msgs = 10000
        self.tolerance = self.n_msgs * 0.01

    def tearDown (self):
        self.tb = None

    def test_001_50percent (self):
        self.drop.set_prob_drop(0.5)

        self.tb.start()
        for emit in range(0,self.n_msgs):
            #time.sleep(.00001)
            self.emitter.emit(self.in_msg)

        time.sleep(.1)
        self.tb.stop()
        self.tb.wait()

        self.assertTrue( abs(self.debug.num_messages() - (self.n_msgs*0.5)) < self.tolerance)
        self.assertEqual( self.debug.num_messages(), self.drop.get_pass_count())
        self.assertEqual( self.n_msgs, self.drop.get_msg_count() )
        self.assertEqual( self.n_msgs - self.debug.num_messages(), self.drop.get_drop_count() )

    def test_002_10percent (self):
        self.drop.set_prob_drop(0.1)

        self.tb.start()
        for emit in range(0,self.n_msgs):
            #time.sleep(.00001)
            self.emitter.emit(self.in_msg)

        time.sleep(.1)
        self.tb.stop()
        self.tb.wait()

        self.assertTrue(abs(self.debug.num_messages() - (self.n_msgs*0.9)) < self.tolerance)
        self.assertEqual( self.debug.num_messages(), self.drop.get_pass_count())
        self.assertEqual( self.n_msgs, self.drop.get_msg_count() )
        self.assertEqual( self.n_msgs - self.debug.num_messages(), self.drop.get_drop_count() )

    def test_003_95percent (self):
        self.drop.set_prob_drop(0.95)

        self.tb.start()
        for emit in range(0,self.n_msgs):
            #time.sleep(.00001)
            self.emitter.emit(self.in_msg)

        time.sleep(.1)
        self.tb.stop()
        self.tb.wait()

        self.assertTrue(abs(self.debug.num_messages() - (self.n_msgs*0.05)) < self.tolerance)
        self.assertEqual( self.debug.num_messages(), self.drop.get_pass_count())
        self.assertEqual( self.n_msgs, self.drop.get_msg_count() )
        self.assertEqual( self.n_msgs - self.debug.num_messages(), self.drop.get_drop_count() )



if __name__ == '__main__':
    gr_unittest.run(qa_msg_drop_random)
