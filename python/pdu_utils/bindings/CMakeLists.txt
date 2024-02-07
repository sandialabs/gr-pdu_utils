# Copyright 2021 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

########################################################################
# Check if there is C++ code at all
########################################################################
if(NOT pdu_utils_sources)
    MESSAGE(STATUS "No C++ sources... skipping python bindings")
    return()
endif(NOT pdu_utils_sources)

########################################################################
# Check for pygccxml
########################################################################
GR_PYTHON_CHECK_MODULE_RAW(
    "pygccxml"
    "import pygccxml"
    PYGCCXML_FOUND
    )

include(GrPybind)

########################################################################
# Python Bindings
########################################################################

list(APPEND pdu_utils_python_files
    constants_python.cc
    extract_metadata_python.cc
    message_counter_python.cc
    message_emitter_python.cc
    message_gate_python.cc
    message_keep_1_in_n_python.cc
    msg_drop_random_python.cc
    pack_unpack_python.cc
    pdu_add_noise_python.cc
    pdu_align_python.cc
    pdu_binary_tools_python.cc
    pdu_burst_combiner_python.cc
    pdu_clock_recovery_python.cc
    pdu_complex_to_mag2_python.cc
    pdu_downsample_python.cc
    pdu_fine_time_measure_python.cc
    pdu_fir_filter_python.cc
    pdu_flow_ctrl_helper_python.cc
    pdu_gmsk_fc_python.cc
    pdu_head_tail_python.cc
    pdu_length_filter_python.cc
    pdu_logger_python.cc
    pdu_pfb_resamp_python.cc
    pdu_preamble_python.cc
    pdu_quadrature_demod_cf_python.cc
    pdu_range_filter_python.cc
    pdu_rotate_python.cc
    pdu_round_robin_python.cc
    pdu_slice_python.cc
    tag_message_trigger_python.cc
    upsample_python.cc
    access_code_to_pdu_python.cc python_bindings.cc)

GR_PYBIND_MAKE_OOT(pdu_utils
   ../../..
   gr::pdu_utils
   "${pdu_utils_python_files}")

# copy bindings extension for use in QA test module
add_custom_command(TARGET pdu_utils_python POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:pdu_utils_python>
    ${PROJECT_BINARY_DIR}/test_modules/gnuradio/pdu_utils/
)

install(TARGETS pdu_utils_python DESTINATION ${GR_PYTHON_DIR}/gnuradio/pdu_utils COMPONENT pythonapi)
