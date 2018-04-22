# Copyright 2011,2012 Free Software Foundation, Inc.
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
# Invoke macro to generate various sources and headers
########################################################################
include(GrMiscUtils)
GR_EXPAND_X_CC_H(pdu_utils pdu_to_bursts_X_impl              c f s b)
GR_EXPAND_X_CC_H(pdu_utils tags_to_pdu_X_impl                c f s b)
GR_EXPAND_X_CC_H(pdu_utils tag_message_trigger_X_impl        c f i s b)
GR_EXPAND_X_CC_H(pdu_utils take_skip_to_pdu_X_impl           c f s b)

########################################################################
# Setup library
########################################################################
include(GrPlatform) #define LIB_SUFFIX

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})
list(APPEND pdu_utils_sources
    ${generated_sources}
    pdu_gmsk_fc_impl.cc
    pdu_set_m_impl.cc
    pdu_burst_combiner_impl.cc
    pdu_split_impl.cc
    pdu_fir_filter_fff_impl.cc
    pdu_pfb_resamp_ff_impl.cc
    message_counter_impl.cc
    message_gate_impl.cc
    message_emitter_impl.cc
    message_keep_1_in_n_impl.cc
    pdu_preamble_impl.cc
    pack_unpack_impl.cc
    extract_metadata_impl.cc
    upsample_impl.cc
    pdu_add_noise_impl.cc
    msg_drop_random_impl.cc
    pdu_head_tail_impl.cc
    time_delta_impl.cc )

set(pdu_utils_sources "${pdu_utils_sources}" PARENT_SCOPE)
if(NOT pdu_utils_sources)
	MESSAGE(STATUS "No C++ sources... skipping lib/")
	return()
endif(NOT pdu_utils_sources)

list(APPEND filter_libs
    gnuradio-runtime
    gnuradio-filter
    gnuradio-blocks
)

add_library(gnuradio-pdu_utils SHARED ${pdu_utils_sources})
target_link_libraries(gnuradio-pdu_utils ${Boost_LIBRARIES} ${LOG4CPP_LIBRARIES} ${GNURADIO_ALL_LIBRARIES})
set_target_properties(gnuradio-pdu_utils PROPERTIES DEFINE_SYMBOL "gnuradio_pdu_utils_EXPORTS")
add_dependencies(gnuradio-pdu_utils gnuradio-filter
  pdu_utils_generated_includes pdu_utils_generated_swigs
  gnuradio-runtime)

if(APPLE)
    set_target_properties(gnuradio-pdu_utils PROPERTIES
        INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
    )
endif(APPLE)

########################################################################
# Install built library files
########################################################################
include(GrMiscUtils)
GR_LIBRARY_FOO(gnuradio-pdu_utils RUNTIME_COMPONENT "pdu_utils_runtime" DEVEL_COMPONENT "pdu_utils_devel")


########################################################################
# Build and register unit test
########################################################################
include(GrTest)

include_directories(${CPPUNIT_INCLUDE_DIRS})

list(APPEND test_pdu_utils_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/test_pdu_utils.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_pdu_utils.cc
)

add_executable(test-pdu_utils ${test_pdu_utils_sources})

target_link_libraries(
  test-pdu_utils
  ${GNURADIO_RUNTIME_LIBRARIES}
  ${Boost_LIBRARIES}
  ${CPPUNIT_LIBRARIES}
  gnuradio-pdu_utils
)

GR_ADD_TEST(test_pdu_utils test-pdu_utils)

########################################################################
# Print summary
########################################################################
message(STATUS "Using install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "Building for version: ${VERSION} / ${LIBVER}")
