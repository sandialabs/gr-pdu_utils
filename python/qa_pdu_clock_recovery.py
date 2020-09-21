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
import numpy as np
import pmt

class qa_pdu_clock_recovery(gr_unittest.TestCase):

    def setUp(self):
        self.tb = gr.top_block()
        self.pmt_sample_rate = pmt.intern("sample_rate")
        self.pmt_symbol_rate = pmt.intern("symbol_rate")


    def tearDown(self):
        self.tb = None

    def test_monte_carlo(self):
      emitter = pdu_utils.message_emitter()
      clock_rec = pdu_utils.pdu_clock_recovery(True)
      msg_debug = blocks.message_debug()

      # make connections
      self.tb.msg_connect((emitter,'msg'),(clock_rec,'pdu_in'))
      self.tb.msg_connect((clock_rec,'pdu_out'),(msg_debug,'store'))

      # run
      self.tb.start()
      time.sleep(.05)

      # generate and emit
      for i in range(100):
        n_symbols = 100
        sps = 8
        noise_power = 0.02 * i
        original_bits = np.random.randint(0,2,n_symbols)
        original_samples = original_bits*2-1
        sample_rate = 1e6
        symbol_rate = sample_rate/sps
        data = np.repeat(original_samples, sps) + (np.random.rand(n_symbols*sps)*np.sqrt(noise_power))

        meta = pmt.make_dict()
        meta = pmt.dict_add(meta, self.pmt_sample_rate, pmt.from_double(1e6))
        vector = pmt.init_f32vector(len(data), data)

        emitter.emit(pmt.cons(meta,vector))

        time.sleep(.05)

        result = msg_debug.get_message(i)
        result_meta = pmt.car(result)
        result_vector = pmt.to_python(pmt.cdr(result))
        n_errors = sum(original_bits[:len(result_vector)] ^ result_vector[:len(original_bits)])
        result_rate = pmt.to_double(pmt.dict_ref(result_meta, self.pmt_symbol_rate, pmt.PMT_NIL))

        #print("result is ", result_rate)
        #print("we expected ", symbol_rate)
        #print("result vector is", result_vector)
        #print("we expected ", original_bits)
        #print("num errors", n_errors)

        # assert some stuff
        if n_errors != 0:
          print ("got bad data", i)
        if (result_rate - symbol_rate) > 100:
          print ("got bad rate", i)
        

      # shut down
      self.tb.stop()
      self.tb.wait()

      self.assertTrue(True)

if __name__ == '__main__':
    gr_unittest.run(qa_pdu_clock_recovery)
