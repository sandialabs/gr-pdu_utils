/*
 * Copyright 2021 Free Software Foundation, Inc.
 *
 * This file is part of GNU Radio
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include <pybind11/pybind11.h>

#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/arrayobject.h>

namespace py = pybind11;

// Headers for binding functions
/**************************************/
// The following comment block is used for
// gr_modtool to insert function prototypes
// Please do not delete
/**************************************/
// BINDING_FUNCTION_PROTOTYPES(
void bind_constants(py::module& m);
void bind_extract_metadata(py::module& m);
void bind_message_counter(py::module& m);
void bind_message_emitter(py::module& m);
void bind_message_gate(py::module& m);
void bind_message_keep_1_in_n(py::module& m);
void bind_msg_drop_random(py::module& m);
void bind_pack_unpack(py::module& m);
void bind_pdu_add_noise(py::module& m);
void bind_pdu_align(py::module& m);
void bind_pdu_binary_tools(py::module& m);
void bind_pdu_burst_combiner(py::module& m);
void bind_pdu_clock_recovery(py::module& m);
void bind_pdu_complex_to_mag2(py::module& m);
void bind_pdu_downsample(py::module& m);
void bind_pdu_fine_time_measure(py::module& m);
void bind_pdu_fir_filter(py::module& m);
void bind_pdu_gmsk_fc(py::module& m);
void bind_pdu_head_tail(py::module& m);
void bind_pdu_length_filter(py::module& m);
void bind_pdu_logger(py::module& m);
void bind_pdu_pfb_resamp(py::module& m);
void bind_pdu_preamble(py::module& m);
void bind_pdu_quadrature_demod_cf(py::module& m);
void bind_pdu_range_filter(py::module& m);
void bind_pdu_rotate(py::module& m);
void bind_pdu_round_robin(py::module& m);
void bind_pdu_set_m(py::module& m);
void bind_pdu_split(py::module& m);
void bind_pdu_to_bursts(py::module& m);
void bind_tag_message_trigger(py::module& m);
void bind_tags_to_pdu(py::module& m);
void bind_take_skip_to_pdu(py::module& m);
void bind_upsample(py::module& m);
void bind_pdu_slice(py::module& m);
void bind_access_code_to_pdu(py::module& m);
// ) END BINDING_FUNCTION_PROTOTYPES


// We need this hack because import_array() returns NULL
// for newer Python versions.
// This function is also necessary because it ensures access to the C API
// and removes a warning.
void* init_numpy()
{
    import_array();
    return NULL;
}

PYBIND11_MODULE(pdu_utils_python, m)
{
    // Initialize the numpy C API
    // (otherwise we will see segmentation faults)
    init_numpy();

    // Allow access to base block methods
    py::module::import("gnuradio.gr");

    /**************************************/
    // The following comment block is used for
    // gr_modtool to insert binding function calls
    // Please do not delete
    /**************************************/
    // BINDING_FUNCTION_CALLS(
    bind_constants(m);
    bind_extract_metadata(m);
    bind_message_counter(m);
    bind_message_emitter(m);
    bind_message_gate(m);
    bind_message_keep_1_in_n(m);
    bind_msg_drop_random(m);
    bind_pack_unpack(m);
    bind_pdu_add_noise(m);
    bind_pdu_align(m);
    bind_pdu_binary_tools(m);
    bind_pdu_burst_combiner(m);
    bind_pdu_clock_recovery(m);
    bind_pdu_complex_to_mag2(m);
    bind_pdu_downsample(m);
    bind_pdu_fine_time_measure(m);
    bind_pdu_fir_filter(m);
    bind_pdu_gmsk_fc(m);
    bind_pdu_head_tail(m);
    bind_pdu_length_filter(m);
    bind_pdu_logger(m);
    bind_pdu_pfb_resamp(m);
    bind_pdu_preamble(m);
    bind_pdu_quadrature_demod_cf(m);
    bind_pdu_range_filter(m);
    bind_pdu_rotate(m);
    bind_pdu_round_robin(m);
    bind_pdu_set_m(m);
    bind_pdu_split(m);
    bind_pdu_to_bursts(m);
    bind_tag_message_trigger(m);
    bind_tags_to_pdu(m);
    bind_take_skip_to_pdu(m);
    bind_upsample(m);
    bind_pdu_slice(m);
    bind_access_code_to_pdu(m);
    // ) END BINDING_FUNCTION_CALLS
}
