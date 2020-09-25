/* -*- c++ -*- */
/*
 * Copyright 2018, 2019, 2020 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef _QA_PDU_UTILS_H_
#define _QA_PDU_UTILS_H_

#include <gnuradio/attributes.h>
#include <cppunit/TestSuite.h>

//! collect all the tests for the gr-filter directory

class __GR_ATTR_EXPORT qa_pdu_utils
{
public:
    //! return suite of tests for all of gr-filter directory
    static CppUnit::TestSuite* suite();
};

#endif /* _QA_PDU_UTILS_H_ */
