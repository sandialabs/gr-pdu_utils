#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright 2018 <+YOU OR YOUR COMPANY+>.
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


class qa_pdu_clock_recovery (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()
        
        # common input and output
        self.emitter = pdu_utils.message_emitter()
        self.debug = blocks.message_debug()
        
        # Device under Test
        self.dut = pdu_utils.pdu_clock_recovery(True)
        
        # Hook it up
        self.tb.msg_connect((self.emitter, 'msg'), (self.dut, 'pdu_in'))
        self.tb.msg_connect((self.dut, 'pdu_out'), (self.debug, 'store'))

    def tearDown (self):
        self.tb = None

    #############
    # basic operational test. 
    # perfect square wave input
    # 4 samples per symbol.
    # Sample size exactly matches FFT size
    def test_001_basicSquare(self):
      print 'Running test_001_basicSquare'
      
      # generate a sqare wave, 4 samples per symbol
      rawData = []
      rawOut = []
      for i in range(0, 256):
          rawData.append(0.5);
          rawData.append(0.5);
          rawData.append(0.5);
          rawData.append(0.5);
          rawData.append(-0.5);
          rawData.append(-0.5);
          rawData.append(-0.5);
          rawData.append(-0.5);
          
          rawOut.append(1);
          rawOut.append(0);

      i_vec = pmt.init_f32vector( len(rawData), rawData )
      e_vec = pmt.init_u8vector( len(rawOut), rawOut )

      
      # input meta
      inMeta = pmt.make_dict()
      inMeta = pmt.dict_add(inMeta, pmt.intern("sample_rate"), pmt.from_float(100000))
      in_pdu = pmt.cons(inMeta, i_vec)
      
      # output meta
      outMeta = pmt.make_dict()
      outMeta = pmt.dict_add( outMeta, pmt.intern("symbol_rate"), pmt.from_float(25000))
      e_pdu = pmt.cons( outMeta, e_vec)

      self.tb.start()
      time.sleep(.001)
      self.emitter.emit(in_pdu)
      time.sleep(.01)
      self.tb.stop()
      self.tb.wait()

      #print "\nEXPECTED: " + repr(pmt.car(e_pdu))
      #print "GOT:      " + repr(pmt.car(self.debug.get_message(0)))
      #print "\nEXPECTED: " + repr(pmt.u8vector_elements(pmt.cdr(e_pdu)))
      #print "GOT:      " + repr(pmt.u8vector_elements(pmt.cdr(self.debug.get_message(0))))

      self.assertEquals( self.debug.num_messages(), 1 )
      self.assertTrue(pmt.equal(self.debug.get_message(0), e_pdu))
      
      
    #############
    # basic operational test. 
    # perfect square wave input
    # 4 samples per symbol.
    # Sample size is not a match of FFT size
    def test_001_offSizeSquare(self):
      print 'Running test_001_offSizeSquare'
      
      # generate a sqare wave, 4 samples per symbol
      rawData = []
      rawOut = []
      for i in range(0, 200):
          rawData.append(0.5);
          rawData.append(0.5);
          rawData.append(0.5);
          rawData.append(0.5);
          rawData.append(-0.5);
          rawData.append(-0.5);
          rawData.append(-0.5);
          rawData.append(-0.5);
          
          rawOut.append(1);
          rawOut.append(0);

      i_vec = pmt.init_f32vector( len(rawData), rawData )
      e_vec = pmt.init_u8vector( len(rawOut), rawOut )

      
      # input meta
      inMeta = pmt.make_dict()
      inMeta = pmt.dict_add(inMeta, pmt.intern("sample_rate"), pmt.from_float(100000))
      in_pdu = pmt.cons(inMeta, i_vec)
      
      # output meta
      outMeta = pmt.make_dict()
      outMeta = pmt.dict_add( outMeta, pmt.intern("symbol_rate"), pmt.from_float(25000))
      e_pdu = pmt.cons( outMeta, e_vec)

      self.tb.start()
      time.sleep(.001)
      self.emitter.emit(in_pdu)
      time.sleep(.01)
      self.tb.stop()
      self.tb.wait()

      #print "\nEXPECTED: " + repr(pmt.car(e_pdu))
      #print "GOT:      " + repr(pmt.car(self.debug.get_message(0)))
      #print "\nEXPECTED: " + repr(pmt.u8vector_elements(pmt.cdr(e_pdu)))
      #print "GOT:      " + repr(pmt.u8vector_elements(pmt.cdr(self.debug.get_message(0))))

      self.assertEquals( self.debug.num_messages(), 1 )
      self.assertTrue(pmt.equal(self.debug.get_message(0), e_pdu))
      

    #############
    # anti test, input is a pair
    def test_002_inputCheck_pair(self):
      
      # generate a sqare wave, 4 samples per symbol
      rawData = []
      rawOut = []
      for i in range(0, 256):
          rawData.append(0.5);
          rawData.append(0.5);
          rawData.append(0.5);
          rawData.append(0.5);
          rawData.append(-0.5);
          rawData.append(-0.5);
          rawData.append(-0.5);
          rawData.append(-0.5);
          
          rawOut.append(1);
          rawOut.append(0);

      i_vec = pmt.init_f32vector( len(rawData), rawData )
      
      in_pdu = i_vec
      
      

      self.tb.start()
      time.sleep(.001)
      self.emitter.emit(in_pdu)
      time.sleep(.01)
      self.tb.stop()
      self.tb.wait()

      #print "\nEXPECTED: " + repr(pmt.car(e_pdu))
      #print "GOT:      " + repr(pmt.car(self.debug.get_message(0)))
      #print "\nEXPECTED: " + repr(pmt.u8vector_elements(pmt.cdr(e_pdu)))
      #print "GOT:      " + repr(pmt.u8vector_elements(pmt.cdr(self.debug.get_message(0))))

      self.assertEquals( self.debug.num_messages(), 0 )
      
    #############
    # anti input test, float
    def test_003_inputCheck_float(self):
      
      # generate a sqare wave, 4 samples per symbol
      rawData = []
      rawOut = []
      for i in range(0, 256):
          rawData.append(0.5);
          rawData.append(0.5);
          rawData.append(0.5);
          rawData.append(0.5);
          rawData.append(-0.5);
          rawData.append(-0.5);
          rawData.append(-0.5);
          rawData.append(-0.5);
          
          rawOut.append(1);
          rawOut.append(0);

      i_vec = pmt.init_u8vector( len(rawOut), rawOut )
      
      # input meta
      inMeta = pmt.make_dict()
      inMeta = pmt.dict_add(inMeta, pmt.intern("sample_rate"), pmt.from_float(100000))
      in_pdu = pmt.cons(inMeta, i_vec)
      
      self.tb.start()
      time.sleep(.001)
      self.emitter.emit(in_pdu)
      time.sleep(.01)
      self.tb.stop()
      self.tb.wait()


      self.assertEquals( self.debug.num_messages(), 0 )
      
    #############
    # anti input test, float
    def test_004_inputCheck_dict(self):
      
      # generate a sqare wave, 4 samples per symbol
      rawData = []
      rawOut = []
      for i in range(0, 256):
          rawData.append(0.5);
          rawData.append(0.5);
          rawData.append(0.5);
          rawData.append(0.5);
          rawData.append(-0.5);
          rawData.append(-0.5);
          rawData.append(-0.5);
          rawData.append(-0.5);
          
          rawOut.append(1);
          rawOut.append(0);

      i_vec = pmt.init_f32vector( len(rawData), rawData )
      e_vec = pmt.init_u8vector( len(rawOut), rawOut )

      
      # input meta
      in_pdu = pmt.cons(e_vec, i_vec)
      
      
      self.tb.start()
      time.sleep(.001)
      self.emitter.emit(in_pdu)
      time.sleep(.01)
      self.tb.stop()
      self.tb.wait()

      
      self.assertEquals( self.debug.num_messages(), 0 )
      
    #############
    # anti input test, 
    def test_005_inputCheck_sampleRate(self):
      
      # generate a sqare wave, 4 samples per symbol
      rawData = []
      rawOut = []
      for i in range(0, 256):
          rawData.append(0.5);
          rawData.append(0.5);
          rawData.append(0.5);
          rawData.append(0.5);
          rawData.append(-0.5);
          rawData.append(-0.5);
          rawData.append(-0.5);
          rawData.append(-0.5);
          
          rawOut.append(1);
          rawOut.append(0);

      i_vec = pmt.init_f32vector( len(rawData), rawData )
      e_vec = pmt.init_u8vector( len(rawOut), rawOut )

      
      # input meta
      inMeta = pmt.make_dict()
      inMeta = pmt.dict_add(inMeta, pmt.intern("magic"), pmt.from_float(100000))
      in_pdu = pmt.cons(inMeta, i_vec)
      
      # output meta
      outMeta = pmt.make_dict()
      outMeta = pmt.dict_add( outMeta, pmt.intern("symbol_rate"), pmt.from_float(25000))
      e_pdu = pmt.cons( outMeta, e_vec)

      self.tb.start()
      time.sleep(.001)
      self.emitter.emit(in_pdu)
      time.sleep(.01)
      self.tb.stop()
      self.tb.wait()

      #print "\nEXPECTED: " + repr(pmt.car(e_pdu))
      #print "GOT:      " + repr(pmt.car(self.debug.get_message(0)))
      #print "\nEXPECTED: " + repr(pmt.u8vector_elements(pmt.cdr(e_pdu)))
      #print "GOT:      " + repr(pmt.u8vector_elements(pmt.cdr(self.debug.get_message(0))))

      self.assertEquals( self.debug.num_messages(), 0 )
      
if __name__ == '__main__':
    gr_unittest.run(qa_pdu_clock_recovery, "qa_pdu_clock_recovery.xml")
