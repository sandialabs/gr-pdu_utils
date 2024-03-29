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
/* BINDTOOL_HEADER_FILE(message_keep_1_in_n.h)                                        */
/* BINDTOOL_HEADER_FILE_HASH(6035798df16e3118a514b5d9d095eec0)                     */
/***********************************************************************************/

#include <pybind11/complex.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#include <gnuradio/pdu_utils/message_keep_1_in_n.h>
// pydoc.h is automatically generated in the build directory
#include <message_keep_1_in_n_pydoc.h>

void bind_message_keep_1_in_n(py::module& m)
{

    using message_keep_1_in_n = ::gr::pdu_utils::message_keep_1_in_n;


    py::class_<message_keep_1_in_n,
               gr::block,
               gr::basic_block,
               std::shared_ptr<message_keep_1_in_n>>(
        m, "message_keep_1_in_n", D(message_keep_1_in_n))

        .def(py::init(&message_keep_1_in_n::make),
             py::arg("n"),
             D(message_keep_1_in_n, make))


        .def("set_n",
             &message_keep_1_in_n::set_n,
             py::arg("n"),
             D(message_keep_1_in_n, set_n))


        .def("get_n", &message_keep_1_in_n::get_n, D(message_keep_1_in_n, get_n))

        ;
}
