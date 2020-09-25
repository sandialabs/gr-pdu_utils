#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright 2018, 2019, 2020 National Technology & Engineering Solutions of Sandia, LLC
# (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
# retains certain rights in this software.
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

import pdu_utils_swig as pdu_utils
from gnuradio import gr, gr_unittest
from gnuradio import blocks
import pmt
import time


class qa_pack_unpack (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()
        self.emitter = pdu_utils.message_emitter()
        self.pack = pdu_utils.pack_unpack(pdu_utils.MODE_PACK_BYTE, pdu_utils.BIT_ORDER_MSB_FIRST)
        self.debug = blocks.message_debug()
        self.tb.msg_connect((self.emitter, 'msg'), (self.pack, 'pdu_in'))
        self.tb.msg_connect((self.pack, 'pdu_out'), (self.debug, 'store'))


    def tearDown (self):
        self.tb = None

    def test_001_pack_MSB (self):
        in_data = [0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1]
        expected_data = [0, 255, 85]
        in_pdu = pmt.cons(pmt.make_dict(), pmt.init_u8vector(len(in_data), in_data))
        expected_pdu = pmt.cons(pmt.make_dict(), pmt.init_u8vector(len(expected_data), expected_data))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(pmt.intern("MALFORMED PDU"))
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        self.assertTrue(pmt.equal(self.debug.get_message(0), expected_pdu))


    def test_002_pack_LSB (self):
        self.pack.set_mode(pdu_utils.MODE_PACK_BYTE)
        self.pack.set_bit_order(pdu_utils.BIT_ORDER_LSB_FIRST)

        in_data = [0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0]
        expected_data = [0, 255, 170, 0]
        in_pdu = pmt.cons(pmt.make_dict(), pmt.init_u8vector(len(in_data), in_data))
        expected_pdu = pmt.cons(pmt.make_dict(), pmt.init_u8vector(len(expected_data), expected_data))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(pmt.cons(pmt.PMT_T, pmt.intern("ANOTHER MALFORMED PDU")))
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        self.assertTrue(pmt.equal(self.debug.get_message(0), expected_pdu))


    def test_003_unpack_MSB (self):
        self.pack.set_mode(pdu_utils.MODE_UNPACK_BYTE)
        self.pack.set_bit_order(pdu_utils.BIT_ORDER_MSB_FIRST)

        in_data = [123, 209, 17, 35]
        expected_data = [0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1]
        in_pdu = pmt.cons(pmt.make_dict(), pmt.init_u8vector(len(in_data), in_data))
        expected_pdu = pmt.cons(pmt.make_dict(), pmt.init_u8vector(len(expected_data), expected_data))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(pmt.cons(pmt.intern("NON U8 PDU"), pmt.init_u16vector(2,[1,2])))
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        self.assertTrue(pmt.equal(self.debug.get_message(0), expected_pdu))


    def test_004_unpack_LSB (self):
        self.pack.set_mode(pdu_utils.MODE_UNPACK_BYTE)
        self.pack.set_bit_order(pdu_utils.BIT_ORDER_LSB_FIRST)

        in_data = [123, 209, 17, 35]
        expected_data = [1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0]
        in_pdu = pmt.cons(pmt.make_dict(), pmt.init_u8vector(len(in_data), in_data))
        expected_pdu = pmt.cons(pmt.make_dict(), pmt.init_u8vector(len(expected_data), expected_data))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(pmt.cons(pmt.intern("NON U8 PDU"), pmt.init_f32vector(2,[1.9,2])))
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        self.assertTrue(pmt.equal(self.debug.get_message(0), expected_pdu))


    def test_005_pack_sparse (self):
        self.pack.set_mode(pdu_utils.MODE_PACK_BYTE)
        self.pack.set_bit_order(pdu_utils.BIT_ORDER_MSB_FIRST)

        in_data = [0, 23, 0, 255, 1, 3]
        expected_data = [92]
        in_pdu = pmt.cons(pmt.make_dict(), pmt.init_u8vector(len(in_data), in_data))
        expected_pdu = pmt.cons(pmt.make_dict(), pmt.init_u8vector(len(expected_data), expected_data))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()

        self.assertTrue(pmt.equal(self.debug.get_message(0), expected_pdu))

    def test_006_bitswap (self):
        self.pack.set_mode(pdu_utils.MODE_BITSWAP_BYTE)

        in_data = [0x7b, 0xd1, 0x11, 0x23]
        expected_data = [0xde, 0x8b, 0x88, 0xc4]
        in_pdu = pmt.cons(pmt.make_dict(), pmt.init_u8vector(len(in_data), in_data))
        expected_pdu = pmt.cons(pmt.make_dict(), pmt.init_u8vector(len(expected_data), expected_data))

        self.tb.start()
        time.sleep(.001)
        self.emitter.emit(pmt.cons(pmt.intern("NON U8 PDU"), pmt.init_u16vector(2,[1,2])))
        time.sleep(.001)
        self.emitter.emit(in_pdu)
        time.sleep(.01)
        self.tb.stop()
        self.tb.wait()
        
        self.assertTrue(pmt.equal(self.debug.get_message(0), expected_pdu))

if __name__ == '__main__':
    gr_unittest.run(qa_pack_unpack)
