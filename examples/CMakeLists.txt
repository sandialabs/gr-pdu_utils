# Copyright 2011 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# GNU Radio is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# GNU Radio is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.

########################################################################
# example applications that get installed into the share path
########################################################################
SET(example_sources
    pdu_filters.grc
    pdu_set_m_controller.py
    pdu_to_burst_test.grc
    pdu_utils_simple_test.py
    pdu_utils_test.grc
)

INSTALL(DIRECTORY test
    DESTINATION ${CMAKE_INSTALL_PREFIX}/${GR_PKG_DATA_DIR}/)

FOREACH(example_source ${example_sources})
    GET_FILENAME_COMPONENT(example_name ${example_source} NAME)
    INSTALL(FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/${example_name}
        DESTINATION ${CMAKE_INSTALL_PREFIX}/${GR_PKG_DATA_DIR}
        COMPONENT examples
    )
ENDFOREACH(example_source)
