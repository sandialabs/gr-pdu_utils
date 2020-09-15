title: The PDU_UTILS OOT Module
brief: Tools for manipulation of PDU objects
tags: # Tags are arbitrary, but look at CGRAN what other authors are using
  - PDU
author:
  - Jacob Gilbert
copyright_owner:
  - Jacob Gilbert
license:
gr_supported_version: v3.7, v3.8
#repo: # Put the URL of the repository here, or leave blank for default
#website: <module_website> # If you have a separate project website, put it here
#icon: <icon_url> # Put a URL to a square image here that will be used as an icon on CGRAN
---
This GNU Radio module contains tools for manipulation of PDU objects. There are blocks to translate between streams and PDUs while maintaining timing information, a number of self explanatory blocks that emulate the behavior of some in-tree stream blocks for messages and PDUs, and some other features. This module is complimentary to the gr-timing_utils module and some of the advanced timing features require blocks there.

All blocks are written in C++, and make use of the GR_LOG API, there is no stdout output; there is minimal PMT symbol interning at runtime, and blocks should be thread-safe. QA code exists for most blocks, and when bugs are fixed QA is updated to catch the errors. The general concept of this module, and usage for some of the more unique blocks are described in this document.
