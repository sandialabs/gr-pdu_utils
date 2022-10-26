#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 
# Copyright 2018-2021 National Technology & Engineering Solutions of Sandia, LLC
# (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
# retains certain rights in this software.
# 
# SPDX-License-Identifier: GPL-3.0-or-later
# 

import numpy
from gnuradio import gr
from gnuradio import pdu_utils
import sys

class pdu_flow_ctrl(gr.basic_block):
    """
    Message in GNU Radio lack any flow control. As far as I can tell, 
    the message queues have an infinite length.

    The PDU Flow Controller can be placed in-line (usually right after 
    the source where PDUs are generated). It takes block IDs as parameters. 
    Each time a PDU enters the flow controller, the flow controller 
    checks each block it has an ID for. If any of those blocks have 
    more than the configured number of messages pending, then the PDU 
    in the flow controller is dropped. Otherwise, it passes through.

    """
    def __init__(self, parent, block_ids, max_nmsgs, verbose=False):
        gr.basic_block.__init__(self,
            name="pdu_flow_ctrl",
            in_sig=None,
            out_sig=None)

        '''
        Constructor
        
        @param block_ids - 
        @param max_nmsgs - 
        @param verbose -
        '''
        self.parent = parent
        self.block_ids = block_ids
        self.max_nmsgs = max_nmsgs
        self.n_dropped = 0
        self.verbose = verbose

        self.message_port_register_in(pdu_utils.PMTCONSTSTR__pdu_in())
        self.message_port_register_out(pdu_utils.PMTCONSTSTR__pdu_out())
        self.set_msg_handler(pdu_utils.PMTCONSTSTR__pdu_in(), self.pdu_handler)

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
            self.message_port_pub(pdu_utils.PMTCONSTSTR__pdu_out(), pdu)
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

