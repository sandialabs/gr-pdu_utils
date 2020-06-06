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


import numpy
import ast
import math
from gnuradio import gr
import pmt
from PyQt4 import Qt, QtCore, QtGui
import pprint, re
import string


class qt_pdu_source(gr.sync_block, QtGui.QGroupBox):
    """
    docstring for block qt_pdu_source
    """
    def __init__(self, defaults={"PDU Vector":[0,1,0,1],"PDU Metadata":'{}'}, submit_text="Send", *args):
        gr.sync_block.__init__(self,
            name="qt_pdu_source",
            in_sig=None,
            out_sig=None)
        QtGui.QGroupBox.__init__(self, *args)

        self.defaults = defaults

        self.add_ascii_cr = False

        # Ensure fields are present
        if 'PDU Vector' not in self.defaults:
            self.defaults['PDU Vector'] = []

        if 'PDU Metadata' not in self.defaults:
            self.defaults['PDU Metadata'] = ''

        # Input data types and their parsing functions
        self.data_types = {"Byte Array": self.parse_byte_array,
                           "ASCII":self.parse_ascii,
                           "Binary Array": self.parse_binary_array,
                           "Hex Array":self.parse_hex_array}

        # Set up GUI bits
        self.labels = {}
        self.textinputs = {}
        self.input_types = {}
        self.lay = QtGui.QGridLayout(self)

        # Add PDU Vector Field
        lbl = QtGui.QLabel("PDU Vector")
        txt = QtGui.QLineEdit(str(self.defaults["PDU Vector"]))
        self.lay.addWidget(lbl, 0, 0) # Row 0, Column 0
        self.lay.addWidget(txt, 0, 1) # Row 0, Column 1
        self.labels[0] = lbl
        self.textinputs[0] = txt

        # Add input type drop down list and make ByteArray be the default
        self.input_types = QtGui.QComboBox()
        for dtype,dindex in self.data_types.iteritems():
          self.input_types.addItem(dtype)
        self.input_types.setCurrentIndex(self.input_types.findText("Byte Array"))
        self.lay.addWidget(self.input_types,0,2)

        # Add PDU Metadata Field
        lbl = QtGui.QLabel("PDU Metadata")
        txt = QtGui.QLineEdit(self.defaults["PDU Metadata"])
        self.lay.addWidget(lbl, 1, 0) # Row 0, Column 0
        self.lay.addWidget(txt, 1, 1) # Row 0, Column 1
        self.labels[1] = lbl
        self.textinputs[1] = txt

        # Set window title
        self.setWindowTitle("Many Controls")

        # Add push button
        self.button = QtGui.QPushButton(str(submit_text))
        self.button.pressed.connect(self.button_clicked)
        self.lay.addWidget(self.button, len(defaults.keys()), 2, 1, 1)

        self.message_port_register_out(pmt.intern("pdu_out"));


    def set_ascii_cr(self, cr_on):
        if cr_on:
	  self.add_ascii_cr = True
        else:
	  self.add_ascii_cr = False


    def button_clicked(self):
        # Add metadata elements to PDU
        meta = pmt.make_dict()
        try:
            v_txt = str(self.textinputs[1].text().toUtf8());
            if len(v_txt) > 0:
                meta_dict = ast.literal_eval(v_txt)
                for key,val in meta_dict.items():
                    if type(val) == list or type(val) == tuple:
                        t = pmt.make_vector(len(val),pmt.from_uint64(0))
                        meta = pmt.dict_add(meta,pmt.intern(str(key)),pmt.to_tuple(t))
                    else: # Store as uint64
                        if isinstance(val, float):
                            if val.is_integer():
                                meta = pmt.dict_add(meta,pmt.intern(str(key)),pmt.from_uint64(int(val)))
                            else:
                                meta = pmt.dict_add(meta,pmt.intern(str(key)),pmt.from_double(val))
                        else:
                            meta = pmt.dict_add(meta,pmt.intern(str(key)),pmt.from_uint64(int(val)))
        except ValueError as err:
            print('PDU Parser Error: ',err)
            pass

        # Generate payload data
        v_txt = str(self.textinputs[0].text())
        vec = self.data_types[str(self.input_types.currentText())](v_txt)

        # Publish message
        self.message_port_pub(pmt.intern("pdu_out"),
            pmt.cons( meta, pmt.to_pmt(numpy.array(vec,dtype=numpy.uint8)) ))

    def parse_byte_array(self,v_txt):
        vec = []
        for entry in re.split('\D',v_txt):
            try:
                item = int(entry)
                vec.append(item)
            except:
                pass

        return vec

    def parse_ascii(self,v_txt):
        #print repr(v_txt)
        if self.add_ascii_cr:
	  v_txt = v_txt + "\r\n"
	  #print repr(v_txt)
        vec = [ord(x) for x in v_txt]
	#print repr(vec)
        return vec

    def parse_binary_array(self,v_txt):
        vec = []
        for entry in re.split(',',v_txt):
            try:
                item = int(entry)
                vec.append(item)
            except:
                pass

        # Pack bit
        vec = numpy.packbits(vec)

        return vec

    def parse_hex_array(self,v_txt):
        vec = []

        # Strip leading brackets off
        v_txt = ''.join( c for c in v_txt if  c not in '[]' )
        #print "v_txt: ",v_txt
        # Replace commas with spaces
        allow = string.letters + string.digits
        for entry in re.sub('[^%s]' % allow, ' ', v_txt).split():
            try:
                #print "entry = ", int(entry,16)
                item = int(entry,16)
                vec.append(item)
            except:
                print "some error"
                pass

        return vec

    def work(self, input_items, output_items):
        pass

if __name__ == "__main__":
    print "testing qt_pdu_source"
    app = QtGui.QApplication([])
    src = qt_pdu_source(defaults={"PDU Vector":[0,1,0,1],"PDU Metadata":"{'trailer flags':(0,0,0),'trailer window':1000,'net ID':25}"})
    src.button_clicked()
    print "all done"
