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
# Invoke macro to generate various headers
#######################################################################
include(GrMiscUtils)
GR_EXPAND_X_H(pdu_utils pdu_to_bursts_X              c f s b)
GR_EXPAND_X_H(pdu_utils tags_to_pdu_X                c f s b)
GR_EXPAND_X_H(pdu_utils tag_message_trigger_X        c f i s b)
GR_EXPAND_X_H(pdu_utils take_skip_to_pdu_X           c f s b)

add_custom_target(pdu_utils_generated_includes DEPENDS
    ${generated_includes}
)

########################################################################
# Install public header files
########################################################################
install(FILES
    ${generated_includes}
    api.h
    constants.h
    pdu_gmsk_fc.h
    pdu_set_m.h
    pdu_burst_combiner.h
    pdu_split.h
    pdu_fir_filter_fff.h
    pdu_pfb_resamp_ff.h
    message_counter.h
    message_gate.h
    message_emitter.h
    message_keep_1_in_n.h
    pdu_preamble.h
    pack_unpack.h
    extract_metadata.h
    upsample.h
    pdu_add_noise.h
    msg_drop_random.h
    pdu_head_tail.h
    time_delta.h DESTINATION include/pdu_utils
)
