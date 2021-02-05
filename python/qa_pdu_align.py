#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright 2018-2021 National Technology & Engineering Solutions of Sandia, LLC
# (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
# retains certain rights in this software.
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

try:
    import pdu_utils
except ImportError:
    import os
    import sys
    dirname, filename = os.path.split(os.path.abspath(__file__))
    sys.path.append(os.path.join(dirname, "bindings"))
    import pdu_utils
from gnuradio import gr, gr_unittest
from gnuradio import blocks
import pmt
import time


class qa_pdu_align (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()
        self.emitter = pdu_utils.message_emitter()
        self.debug = blocks.message_debug()
        
    def connectUp(self):
        self.tb.msg_connect((self.emitter, 'msg'), (self.dut, 'pdu_in'))
        self.tb.msg_connect((self.dut, 'pdu_out'), (self.debug, 'store'))

    def tearDown (self):
        self.tb = None

    def test_001_no_match (self):
        
        self.dut = pdu_utils.pdu_align('01010101', 0, 0, pdu_utils.ALIGN_DROP)
        self.connectUp()
        
        in_data = [0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1]
        expected_data = [0]
        in_pdu = pmt.cons(pmt.make_dict(), pmt.init_u8vector(len(in_data), in_data))
        expected_pdu = pmt.cons(pmt.make_dict(), pmt.init_u8vector(len(expected_data), expected_data))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        self.assertEqual(0, self.debug.num_messages())
    
    def test_002_match (self):
        
        self.dut = pdu_utils.pdu_align('01010101', 0, 0, pdu_utils.ALIGN_DROP)
        self.connectUp()
        
        in_data = [0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1]
        expected_data = [ 1, 1, 1, 1, 1, 1, 1, 1 ]
        in_pdu = pmt.cons(pmt.make_dict(), pmt.init_u8vector(len(in_data), in_data))
        expected_pdu = pmt.cons(pmt.make_dict(), pmt.init_u8vector(len(expected_data), expected_data))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        self.assertEqual(1, self.debug.num_messages())
        self.assertTrue(pmt.equal(self.debug.get_message(0), expected_pdu))
        
    def test_003_match_offseta (self):
        
        self.dut = pdu_utils.pdu_align('01010101', 0, 4, pdu_utils.ALIGN_DROP)
        self.connectUp()
        
        in_data = [0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1]
        expected_data = [ 1, 1, 1, 1 ]
        in_pdu = pmt.cons(pmt.make_dict(), pmt.init_u8vector(len(in_data), in_data))
        expected_pdu = pmt.cons(pmt.make_dict(), pmt.init_u8vector(len(expected_data), expected_data))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        self.assertEqual(1, self.debug.num_messages())
        self.assertTrue(pmt.equal(self.debug.get_message(0), expected_pdu))

    def test_004_match_offsetb (self):
        
        self.dut = pdu_utils.pdu_align('01010101', 0, -4, pdu_utils.ALIGN_DROP)
        self.connectUp()
        
        in_data = [0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1]
        expected_data = [ 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1 ]
        in_pdu = pmt.cons(pmt.make_dict(), pmt.init_u8vector(len(in_data), in_data))
        expected_pdu = pmt.cons(pmt.make_dict(), pmt.init_u8vector(len(expected_data), expected_data))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        self.assertEqual(1, self.debug.num_messages())
        self.assertTrue(pmt.equal(self.debug.get_message(0), expected_pdu))
        
    def test_005_match_error0 (self):
        
        self.dut = pdu_utils.pdu_align('01010101', 1, 0, pdu_utils.ALIGN_DROP)
        self.connectUp()
        
        in_data = [0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1]
        expected_data = [ 1, 1, 1, 1, 1, 1, 1, 1 ]
        in_pdu = pmt.cons(pmt.make_dict(), pmt.init_u8vector(len(in_data), in_data))
        expected_pdu = pmt.cons(pmt.make_dict(), pmt.init_u8vector(len(expected_data), expected_data))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()
        
        self.assertEqual(1, self.debug.num_messages())
        
        print("\nEXPECTED: " + repr(pmt.car(expected_pdu)))
        print("GOT:      " + repr(pmt.car(self.debug.get_message(0))))
        print("\nEXPECTED: " + repr(pmt.u8vector_elements(pmt.cdr(expected_pdu))))
        print("GOT:      " + repr(pmt.u8vector_elements(pmt.cdr(self.debug.get_message(0)))))
        
        self.assertTrue(pmt.equal(self.debug.get_message(0), expected_pdu))
        
    def test_005_match_error0b (self):
        
        self.dut = pdu_utils.pdu_align('10101010', 1, 0, pdu_utils.ALIGN_DROP)
        self.connectUp()
        
        in_data = [1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1]
        expected_data = [ 1, 1, 1, 1, 1, 1, 1, 1 ]
        in_pdu = pmt.cons(pmt.make_dict(), pmt.init_u8vector(len(in_data), in_data))
        expected_pdu = pmt.cons(pmt.make_dict(), pmt.init_u8vector(len(expected_data), expected_data))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()
        
        self.assertEqual(1, self.debug.num_messages())
        
        print("\nEXPECTED: " + repr(pmt.car(expected_pdu)))
        print("GOT:      " + repr(pmt.car(self.debug.get_message(0))))
        print("\nEXPECTED: " + repr(pmt.u8vector_elements(pmt.cdr(expected_pdu))))
        print("GOT:      " + repr(pmt.u8vector_elements(pmt.cdr(self.debug.get_message(0)))))
        
        self.assertTrue(pmt.equal(self.debug.get_message(0), expected_pdu))
        
        
    def test_006_match_error1 (self):
        
        self.dut = pdu_utils.pdu_align('01010101', 1, 0, pdu_utils.ALIGN_DROP)
        self.connectUp()
        
        in_data = [1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1]
        expected_data = [ 1, 1, 1, 1, 1, 1, 1, 1 ]
        in_pdu = pmt.cons(pmt.make_dict(), pmt.init_u8vector(len(in_data), in_data))
        expected_pdu = pmt.cons(pmt.make_dict(), pmt.init_u8vector(len(expected_data), expected_data))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        self.assertEqual(1, self.debug.num_messages())
        self.assertTrue(pmt.equal(self.debug.get_message(0), expected_pdu))


    def test_007_match_errors2 (self):
        '''
        Ensure specification of the number of errors in an alignment sequence is
        properly checked
        '''
        self.dut = pdu_utils.pdu_align('10101010', 2, 0, pdu_utils.ALIGN_EMPTY)
        self.connectUp()
        
        in_data = [1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1]
        expected_data = [0, 1, 0, 1, 0, 1, 0, 1]
        
        in_pdu = pmt.cons(pmt.make_dict(), pmt.init_u8vector(len(in_data), in_data))
        expected_pdu = pmt.cons(pmt.make_dict(), pmt.init_u8vector(len(expected_data), expected_data))
        
        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.1)
        self.tb.stop()
        self.tb.wait()
        
        self.assertTrue(pmt.equal(self.debug.get_message(0), expected_pdu))

    def test_008_match_leading_zeros (self):
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
        preamble = '00000000' * 3 + '10110111'
        self.dut = pdu_utils.pdu_align(preamble, 0, 0, pdu_utils.ALIGN_EMPTY)
        self.connectUp()
        
        in_data = [int(a) for a in ('10110111' + '00000000' * 3 + '10110111' + '10100101')]
        expected_data = [1, 0, 1, 0, 0, 1, 0, 1]
        
        in_pdu = pmt.cons(pmt.make_dict(), pmt.init_u8vector(len(in_data), in_data))
        expected_pdu = pmt.cons(pmt.make_dict(), pmt.init_u8vector(len(expected_data), expected_data))
        
        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.1)
        self.tb.stop()
        self.tb.wait()
        
        self.assertTrue(pmt.equal(self.debug.get_message(0), expected_pdu))


if __name__ == '__main__':
    gr_unittest.run(qa_pdu_align, "qa_pdu_align.xml")
