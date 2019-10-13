#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 
# Copyright 2018 gr-pdu_utils author.
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

import numpy
from gnuradio import gr
import pdu_utils
import sys

class pdu_flow_ctrl(gr.basic_block):
    """
    docstring for block pdu_flow_ctrl
    """
    def __init__(self, parent, block_ids, max_nmsgs, verbose=False):
        gr.basic_block.__init__(self,
            name="pdu_flow_ctrl",
            in_sig=None,
            out_sig=None)

        self.parent = parent
        self.block_ids = block_ids
        self.max_nmsgs = max_nmsgs
        self.n_dropped = 0
        self.verbose = verbose

        self.message_port_register_in(pdu_utils.PMTCONSTSTR__PDU_IN)
        self.message_port_register_out(pdu_utils.PMTCONSTSTR__PDU_OUT)
        self.set_msg_handler(pdu_utils.PMTCONSTSTR__PDU_IN, self.pdu_handler)

    def start(self):
        # build the helper
        try:
            blocks = [eval('self.parent.'+block_id+'.to_basic_block()') for block_id in self.block_ids]
        except AttributeError as e:
            print('ERROR: problem setting up PDU Flow Controller:', e, '(was an invalid block_id given?)')
            raise
        self.helper = pdu_utils.pdu_flow_ctrl_helper(blocks)

        # get rid of sptr references
        for i in range(len(blocks)):
            blocks[i] = None
        blocks = None

        self.n_dropped = 0
        return True

    def stop(self):
        print('pdu_flow_ctrl: dropped', self.n_dropped, 'PDUs')
        self.helper = None
        return True

    def pdu_handler(self, pdu):
        # if the max number of allowed messages is greater than the current max number of messages
        if self.helper.max_nmsgs() < self.max_nmsgs:
            self.message_port_pub(pdu_utils.PMTCONSTSTR__PDU_OUT, pdu)
        else:
            self.n_dropped += 1
            if self.verbose:
                self.helper.print_nmsgs()
                print('WARNING: messages backing up, dropping a PDU')
            else:
                sys.stdout.write('F')
                sys.stdout.flush()

    def print_nmsgs(self):
        self.helper.print_nmsgs()

