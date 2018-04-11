# Copyright 2011 Free Software Foundation, Inc.
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
install(FILES
    pdu_utils_tags_to_pdu_X.xml
    pdu_utils_pdu_to_bursts_X.xml
    pdu_utils_qt_pdu_source.xml
    pdu_utils_message_gate.xml
    pdu_utils_pdu_split.xml
    pdu_utils_tag_message_trigger_X.xml
    pdu_utils_pdu_preamble.xml
    pdu_utils_pdu_gmsk_fc.xml
    pdu_utils_pdu_set_m.xml
    pdu_utils_pdu_head_tail.xml
    pdu_utils_pdu_burst_combiner.xml
    pdu_utils_take_skip_to_pdu.xml
    pdu_utils_pdu_fir_filter_fff.xml
    pdu_utils_pdu_pfb_resamp_ff.xml
    pdu_utils_message_counter.xml
    pdu_utils_message_gate.xml
    pdu_utils_message_emitter.xml
    pdu_utils_message_keep_1_in_n.xml
    pdu_utils_pdu_preamble.xml
    pdu_utils_pack_unpack.xml
    pdu_utils_extract_metadata.xml
    pdu_utils_upsample.xml
    pdu_utils_pdu_add_noise.xml
    pdu_utils_msg_drop_random.xml
    pdu_utils_time_delta.xml DESTINATION share/gnuradio/grc/blocks
)
