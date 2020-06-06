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


class qa_pdu_align (gr_unittest.TestCase):

    def setUp (self):
      self.tb = gr.top_block ()
      self.emitter = pdu_utils.message_emitter()
      self.align = pdu_utils.pdu_align('10101010',0, 0, pdu_utils.ALIGN_EMPTY)
      self.debug = blocks.message_debug()
      self.tb.msg_connect((self.emitter, 'msg'), (self.align, 'pdu_in'))
      self.tb.msg_connect((self.align, 'pdu_out'), (self.debug, 'store'))

    def tearDown (self):
      self.tb = None

    def test_001_instantiation (self):
      '''
      Basic block usage
      '''
      in_data = [1,0,1,0,1,0,1,0,0,1,0,1,0,1,0,1]
      expected_data = [0,1,0,1,0,1,0,1]

      in_pdu = pmt.cons(pmt.make_dict(), pmt.init_u8vector(len(in_data), in_data))
      expected_pdu = pmt.cons(pmt.make_dict(), pmt.init_u8vector(len(expected_data), expected_data))

      self.tb.start()
      time.sleep(.001)
      self.emitter.emit(in_pdu)
      time.sleep(.1)
      self.tb.stop()
      self.tb.wait()

      self.assertTrue(pmt.equal(self.debug.get_message(0), expected_pdu))

    def test_002_errors (self):
      '''
      Ensure specification of the number of errors in an alignment sequence is
      properly checked
      '''
      in_data = [1,1,1,1,1,0,1,0,0,1,0,1,0,1,0,1]
      expected_data = [0,1,0,1,0,1,0,1]

      in_pdu = pmt.cons(pmt.make_dict(), pmt.init_u8vector(len(in_data), in_data))
      expected_pdu = pmt.cons(pmt.make_dict(), pmt.init_u8vector(len(expected_data), expected_data))

      tb = gr.top_block ()
      emitter = pdu_utils.message_emitter()
      align = pdu_utils.pdu_align('10101010',2,0, pdu_utils.ALIGN_EMPTY)
      debug = blocks.message_debug()
      tb.msg_connect((emitter, 'msg'), (align, 'pdu_in'))
      tb.msg_connect((align, 'pdu_out'), (debug, 'store'))

      tb.start()
      time.sleep(.001)
      emitter.emit(in_pdu)
      time.sleep(.1)
      tb.stop()
      tb.wait()

      self.assertTrue(pmt.equal(debug.get_message(0), expected_pdu))

    def test_003_leading_zeros (self):
      '''
      Leading zeros at the beginning of the alignment sequence previously caused
      invalid alignments due to not checking to ensure that the entire alignment
      sequence was checked before emitting the message

      For an alignment sequence of:
        '00000000'*3 + '10110111'

      a message was kicked out with the following contents
        '00000000'*3 + '10110111' + ...

      if the first 8 bits of a sequence were within the threshold of the last 8
      bits of the alignment sequence, which has a 1 in 256 chance of occuring
      statistically anyway.

      '''
      preamble = '00000000'*3 + '10110111'
      in_data = [int(a) for a in ('10110111' + '00000000'*3  +'10110111' + '10100101')]
      expected_data = [1,0,1,0,0,1,0,1]

      in_pdu = pmt.cons(pmt.make_dict(), pmt.init_u8vector(len(in_data), in_data))
      expected_pdu = pmt.cons(pmt.make_dict(), pmt.init_u8vector(len(expected_data), expected_data))

      tb = gr.top_block ()
      emitter = pdu_utils.message_emitter()
      align = pdu_utils.pdu_align(preamble,0,0, pdu_utils.ALIGN_EMPTY)
      debug = blocks.message_debug()
      tb.msg_connect((emitter, 'msg'), (align, 'pdu_in'))
      tb.msg_connect((align, 'pdu_out'), (debug, 'store'))

      tb.start()
      time.sleep(.001)
      emitter.emit(in_pdu)
      time.sleep(.1)
      tb.stop()
      tb.wait()

      self.assertTrue(pmt.equal(debug.get_message(0), expected_pdu))

if __name__ == '__main__':
    gr_unittest.run(qa_pdu_align, "qa_pdu_align.xml")
