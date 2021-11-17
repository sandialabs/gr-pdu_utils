/*
 * Copyright 2021 Free Software Foundation, Inc.
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
/* BINDTOOL_HEADER_FILE(pdu_set_m.h)                                        */
/* BINDTOOL_HEADER_FILE_HASH(ebee2a97b9ae1a6d373062f98e20c87c)                     */
/***********************************************************************************/

#include <pybind11/complex.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#include <pdu_utils/pdu_set_m.h>
// pydoc.h is automatically generated in the build directory
#include <pdu_set_m_pydoc.h>

void bind_pdu_set_m(py::module& m)
{

    using pdu_set_m = ::gr::pdu_utils::pdu_set_m;


    py::class_<pdu_set_m, gr::block, gr::basic_block, std::shared_ptr<pdu_set_m>>(
        m, "pdu_set_m", D(pdu_set_m))

        .def(py::init(&pdu_set_m::make), py::arg("k"), py::arg("v"), D(pdu_set_m, make))


        .def("key", &pdu_set_m::key, D(pdu_set_m, key))


        .def("val", &pdu_set_m::val, D(pdu_set_m, val))


        .def("set_key", &pdu_set_m::set_key, py::arg("k"), D(pdu_set_m, set_key))


        .def("set_val", &pdu_set_m::set_val, py::arg("v"), D(pdu_set_m, set_val))


        .def("set_kv",
             &pdu_set_m::set_kv,
             py::arg("k"),
             py::arg("v"),
             D(pdu_set_m, set_kv))

        ;
}