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
/* BINDTOOL_HEADER_FILE(pdu_add_noise.h)                                        */
/* BINDTOOL_HEADER_FILE_HASH(e9d0f9e54a5f526cb738f60d7781640e)                     */
/***********************************************************************************/

#include <pybind11/complex.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#include <pdu_utils/pdu_add_noise.h>
// pydoc.h is automatically generated in the build directory
#include <pdu_add_noise_pydoc.h>

void bind_pdu_add_noise(py::module& m)
{

    using pdu_add_noise = ::gr::pdu_utils::pdu_add_noise;


    py::class_<pdu_add_noise, gr::block, gr::basic_block, std::shared_ptr<pdu_add_noise>>(
        m, "pdu_add_noise", D(pdu_add_noise))

        .def(py::init(&pdu_add_noise::make),
             py::arg("noise_level"),
             py::arg("offset"),
             py::arg("scale"),
             py::arg("seed") = 12345678,
             py::arg("dist") = 0,
             D(pdu_add_noise, make))


        .def("set_noise_level",
             &pdu_add_noise::set_noise_level,
             py::arg("nl"),
             D(pdu_add_noise, set_noise_level))


        .def("set_offset",
             &pdu_add_noise::set_offset,
             py::arg("o"),
             D(pdu_add_noise, set_offset))


        .def("set_scale",
             &pdu_add_noise::set_scale,
             py::arg("s"),
             D(pdu_add_noise, set_scale))


        .def("set_noise_dist",
             &pdu_add_noise::set_noise_dist,
             py::arg("d"),
             D(pdu_add_noise, set_noise_dist))


        .def("set_seed",
             &pdu_add_noise::set_seed,
             py::arg("x"),
             D(pdu_add_noise, set_seed))

        ;
}
