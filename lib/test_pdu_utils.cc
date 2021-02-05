/* -*- c++ -*- */
/*
 * Copyright 2018-2021 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <cppunit/TextTestRunner.h>
#include <cppunit/XmlOutputter.h>

#include "qa_pdu_utils.h"
#include <gnuradio/unittests.h>
#include <fstream>
#include <iostream>

int main(int argc, char** argv)
{
    CppUnit::TextTestRunner runner;
    std::ofstream xmlfile(get_unittest_path("pdu_utils.xml").c_str());
    CppUnit::XmlOutputter* xmlout = new CppUnit::XmlOutputter(&runner.result(), xmlfile);

    runner.addTest(qa_pdu_utils::suite());
    runner.setOutputter(xmlout);

    bool was_successful = runner.run("", false);

    return was_successful ? 0 : 1;
}
