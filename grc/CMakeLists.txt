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
    pdu_utils_tags_to_pdu_X.block.yml
    pdu_utils_pdu_to_bursts_X.block.yml
    pdu_utils_qt_pdu_source.block.yml
    pdu_utils_message_gate.block.yml
    pdu_utils_pdu_split.block.yml
    pdu_utils_tag_message_trigger_X.block.yml
    pdu_utils_pdu_preamble.block.yml
    pdu_utils_pdu_gmsk_fc.block.yml
    pdu_utils_pdu_set_m.block.yml
    pdu_utils_pdu_head_tail.block.yml
    pdu_utils_pdu_burst_combiner.block.yml
    pdu_utils_take_skip_to_pdu.block.yml
    pdu_utils_pdu_fir_filter.block.yml
    pdu_utils_pdu_pfb_resamp.block.yml
    #pdu_utils_pdu_rational_resampler_cc.xml
    pdu_utils_message_counter.block.yml
    pdu_utils_message_gate.block.yml
    pdu_utils_message_emitter.block.yml
    pdu_utils_message_keep_1_in_n.block.yml
    pdu_utils_pdu_preamble.block.yml
    pdu_utils_pack_unpack.block.yml
    pdu_utils_extract_metadata.block.yml
    pdu_utils_upsample.block.yml
    pdu_utils_pdu_add_noise.block.yml
    pdu_utils_msg_drop_random.block.yml
    pdu_utils_time_delta.block.yml
    pdu_utils_pdu_length_filter.block.yml
    pdu_utils_pdu_logger.block.yml
    pdu_utils_pdu_clock_recovery.block.yml
    pdu_utils_pdu_align.block.yml
    pdu_utils_pdu_range_filter.block.yml
    pdu_utils_pdu_round_robin.block.yml
    pdu_utils_pdu_flow_ctrl.block.yml
    pdu_utils_pdu_binary_tools.block.yml
    pdu_utils_pdu_commutator.block.yml
    pdu_utils_pdu_downsample.block.yml
    pdu_utils_pdu_fine_time_measure.block.yml
    pdu_utils_pdu_complex_to_mag2.block.yml DESTINATION share/gnuradio/grc/blocks
)
