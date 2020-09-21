# Copyright 2011 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# SPDX-License-Identifier: GPL-3.0-or-later
install(FILES
    pdu_utils_tags_to_pdu.block.yml
    pdu_utils_pdu_to_bursts.block.yml
    pdu_utils_qt_pdu_source.block.yml
    pdu_utils_message_gate.block.yml
    pdu_utils_pdu_split.block.yml
    pdu_utils_tag_message_trigger.block.yml
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
    pdu_utils_pdu_length_filter.block.yml
    pdu_utils_pdu_logger.block.yml
    pdu_utils_pdu_clock_recovery.block.yml
    pdu_utils_pdu_align.block.yml
    pdu_utils_pdu_range_filter.block.yml
    pdu_utils_pdu_round_robin.block.yml
    pdu_utils_pdu_flow_ctrl.block.yml
    pdu_utils_pdu_binary_tools.block.yml
    pdu_utils_pdu_downsample.block.yml
    pdu_utils_pdu_fine_time_measure.block.yml
    pdu_utils_pdu_complex_to_mag2.block.yml
    pdu_utils_pdu_quadrature_demod_cf.block.yml
    pdu_utils_sandia_message_debug.block.yml DESTINATION share/gnuradio/grc/blocks
)
