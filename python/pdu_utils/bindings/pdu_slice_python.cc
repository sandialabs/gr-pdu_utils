/*
 * Copyright 2022 Free Software Foundation, Inc.
 *
 * This file is part of GNU Radio
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

/***********************************************************************************/
/* This file is automatically generated using bindtool and can be manually edited  */
/* The following lines can be configured to regenerate this file during cmake      */
/* If manual edits are made, the following tags should be modified accordingly.    */
/* BINDTOOL_GEN_AUTOMATIC(0)                                                       */
/* BINDTOOL_USE_PYGCCXML(0)                                                        */
/* BINDTOOL_HEADER_FILE(pdu_slice.h)                                        */
/* BINDTOOL_HEADER_FILE_HASH(c630a9faf1e07aca1a3310d5b3e89e20)                     */
/***********************************************************************************/

#include <pybind11/complex.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#include <gnuradio/pdu_utils/pdu_slice.h>
// pydoc.h is automatically generated in the build directory
#include <pdu_slice_pydoc.h>

void bind_pdu_slice(py::module& m)
{

    using pdu_slice = ::gr::pdu_utils::pdu_slice;


    py::class_<pdu_slice, gr::block, gr::basic_block, std::shared_ptr<pdu_slice>>(
        m, "pdu_slice", D(pdu_slice))

        .def(py::init(&pdu_slice::make), py::arg("slice"), D(pdu_slice, make))


        .def(
            "set_slice", &pdu_slice::set_slice, py::arg("slice"), D(pdu_slice, set_slice))

        ;
}
