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
import os

# import pybind11 generated symbols into the pdu_utils namespace
try:
    from .pdu_utils_python import *
except ImportError:
    dirname, filename = os.path.split(os.path.abspath(__file__))
    __path__.append(os.path.join(dirname, "bindings"))
    from .pdu_utils_python import *

# import any pure python here
from .pdu_flow_ctrl import pdu_flow_ctrl
try: from .qt_pdu_source import qt_pdu_source
except: print("QT PDU Source not available (import PyQt4 failed)")
#
