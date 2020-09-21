#
# Copyright 2008,2009 Free Software Foundation, Inc.
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

# The presence of this file turns this directory into a Python package

'''
This is the GNU Radio PDU_UTILS module. Place your Python package
description here (python/__init__.py).
'''
from __future__ import unicode_literals

# import swig generated symbols into the pdu_utils namespace
try:
    # this might fail if the module is python-only
    from .pdu_utils_swig import *
except ImportError as e:
    print("Import Error: {}".format(e))
    pass

# import any pure python here
from .pdu_flow_ctrl import pdu_flow_ctrl
try: from .qt_pdu_source import qt_pdu_source
except: print("QT PDU Source not available (import PyQt4 failed)")
#
