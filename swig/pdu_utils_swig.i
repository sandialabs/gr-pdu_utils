/* -*- c++ -*- */

#define PDU_UTILS_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "pdu_utils_swig_doc.i"

%{
#include "pdu_utils/constants.h"
#include "pdu_utils/pdu_to_bursts.h"
#include "pdu_utils/tags_to_pdu.h"
#include "pdu_utils/take_skip_to_pdu.h"
#include "pdu_utils/tag_message_trigger.h"
#include "pdu_utils/pdu_gmsk_fc.h"
#include "pdu_utils/pdu_set_m.h"
#include "pdu_utils/pdu_burst_combiner.h"
#include "pdu_utils/pdu_split.h"
#include "pdu_utils/pdu_fir_filter.h"
#include "pdu_utils/pdu_pfb_resamp.h"
/*#include "pdu_utils/pdu_rational_resampler_cc.h"*/
#include "pdu_utils/message_counter.h"
#include "pdu_utils/message_gate.h"
#include "pdu_utils/message_emitter.h"
#include "pdu_utils/message_keep_1_in_n.h"
#include "pdu_utils/pdu_preamble.h"
#include "pdu_utils/pack_unpack.h"
#include "pdu_utils/extract_metadata.h"
#include "pdu_utils/upsample.h"
#include "pdu_utils/pdu_add_noise.h"
#include "pdu_utils/msg_drop_random.h"
#include "pdu_utils/pdu_head_tail.h"
#include "pdu_utils/pdu_length_filter.h"
#include "pdu_utils/pdu_logger.h"
#include "pdu_utils/pdu_clock_recovery.h"
#include "pdu_utils/pdu_align.h"
#include "pdu_utils/pdu_range_filter.h"
#include "pdu_utils/pdu_round_robin.h"
#include "pdu_utils/pdu_flow_ctrl_helper.h"
#include "pdu_utils/pdu_binary_tools.h"
#include "pdu_utils/pdu_downsample.h"
#include "pdu_utils/pdu_fine_time_measure.h"
#include "pdu_utils/pdu_complex_to_mag2.h"
#include "pdu_utils/sandia_message_debug.h"
#include "pdu_utils/pdu_quadrature_demod_cf.h"

%}

%include "pdu_utils/constants.h"

// pdu_to_bursts
%include "pdu_utils/pdu_to_bursts.h"
GR_SWIG_BLOCK_MAGIC2_TMPL(pdu_utils, pdu_to_bursts_b, pdu_to_bursts<unsigned char>);
GR_SWIG_BLOCK_MAGIC2_TMPL(pdu_utils, pdu_to_bursts_s, pdu_to_bursts<short>);
GR_SWIG_BLOCK_MAGIC2_TMPL(pdu_utils, pdu_to_bursts_f, pdu_to_bursts<float>);
GR_SWIG_BLOCK_MAGIC2_TMPL(pdu_utils, pdu_to_bursts_c, pdu_to_bursts<gr_complex>);

// tags_measure_trigger
%include "pdu_utils/tag_message_trigger.h"
GR_SWIG_BLOCK_MAGIC2_TMPL(pdu_utils, tag_message_trigger_b, tag_message_trigger<unsigned char>);
GR_SWIG_BLOCK_MAGIC2_TMPL(pdu_utils, tag_message_trigger_s, tag_message_trigger<short>);
GR_SWIG_BLOCK_MAGIC2_TMPL(pdu_utils, tag_message_trigger_i, tag_message_trigger<int32_t>);
GR_SWIG_BLOCK_MAGIC2_TMPL(pdu_utils, tag_message_trigger_f, tag_message_trigger<float>);
GR_SWIG_BLOCK_MAGIC2_TMPL(pdu_utils, tag_message_trigger_c, tag_message_trigger<gr_complex>);

// tags_to_pdu
%include "pdu_utils/tags_to_pdu.h"
GR_SWIG_BLOCK_MAGIC2_TMPL(pdu_utils, tags_to_pdu_b, tags_to_pdu<unsigned char>);
GR_SWIG_BLOCK_MAGIC2_TMPL(pdu_utils, tags_to_pdu_s, tags_to_pdu<short>);
GR_SWIG_BLOCK_MAGIC2_TMPL(pdu_utils, tags_to_pdu_f, tags_to_pdu<float>);
GR_SWIG_BLOCK_MAGIC2_TMPL(pdu_utils, tags_to_pdu_c, tags_to_pdu<gr_complex>);

// take_skip_to_pdu
%include "pdu_utils/take_skip_to_pdu.h"
GR_SWIG_BLOCK_MAGIC2_TMPL(pdu_utils, take_skip_to_pdu_b, take_skip_to_pdu<unsigned char>);
GR_SWIG_BLOCK_MAGIC2_TMPL(pdu_utils, take_skip_to_pdu_s, take_skip_to_pdu<short>);
GR_SWIG_BLOCK_MAGIC2_TMPL(pdu_utils, take_skip_to_pdu_f, take_skip_to_pdu<float>);
GR_SWIG_BLOCK_MAGIC2_TMPL(pdu_utils, take_skip_to_pdu_c, take_skip_to_pdu<gr_complex>);

%include "pdu_utils/pdu_gmsk_fc.h"
GR_SWIG_BLOCK_MAGIC2(pdu_utils, pdu_gmsk_fc);
%include "pdu_utils/pdu_set_m.h"
GR_SWIG_BLOCK_MAGIC2(pdu_utils, pdu_set_m);
%include "pdu_utils/pdu_burst_combiner.h"
GR_SWIG_BLOCK_MAGIC2(pdu_utils, pdu_burst_combiner);
%include "pdu_utils/pdu_split.h"
GR_SWIG_BLOCK_MAGIC2(pdu_utils, pdu_split);
%include "pdu_utils/pdu_fir_filter.h"
GR_SWIG_BLOCK_MAGIC2(pdu_utils, pdu_fir_filter);

// pdu_pfb_resamp
%include "pdu_utils/pdu_pfb_resamp.h"
GR_SWIG_BLOCK_MAGIC2_TMPL(pdu_utils, pdu_pfb_resamp_fff, pdu_pfb_resamp<float,float>);
GR_SWIG_BLOCK_MAGIC2_TMPL(pdu_utils, pdu_pfb_resamp_ccf, pdu_pfb_resamp<gr_complex,float>);
GR_SWIG_BLOCK_MAGIC2_TMPL(pdu_utils, pdu_pfb_resamp_ccc, pdu_pfb_resamp<gr_complex,gr_complex>);

/*%include "pdu_utils/pdu_rational_resampler_cc.h"
GR_SWIG_BLOCK_MAGIC2(pdu_utils, pdu_rational_resampler_cc);*/
%include "pdu_utils/message_counter.h"
GR_SWIG_BLOCK_MAGIC2(pdu_utils, message_counter);
%include "pdu_utils/message_gate.h"
GR_SWIG_BLOCK_MAGIC2(pdu_utils, message_gate);
%include "pdu_utils/message_emitter.h"
GR_SWIG_BLOCK_MAGIC2(pdu_utils, message_emitter);
%include "pdu_utils/message_keep_1_in_n.h"
GR_SWIG_BLOCK_MAGIC2(pdu_utils, message_keep_1_in_n);
%include "pdu_utils/pdu_preamble.h"
GR_SWIG_BLOCK_MAGIC2(pdu_utils, pdu_preamble);
%include "pdu_utils/pack_unpack.h"
GR_SWIG_BLOCK_MAGIC2(pdu_utils, pack_unpack);
%include "pdu_utils/extract_metadata.h"
GR_SWIG_BLOCK_MAGIC2(pdu_utils, extract_metadata);
%include "pdu_utils/upsample.h"
GR_SWIG_BLOCK_MAGIC2(pdu_utils, upsample);
%include "pdu_utils/pdu_add_noise.h"
GR_SWIG_BLOCK_MAGIC2(pdu_utils, pdu_add_noise);
%include "pdu_utils/msg_drop_random.h"
GR_SWIG_BLOCK_MAGIC2(pdu_utils, msg_drop_random);
%include "pdu_utils/pdu_head_tail.h"
GR_SWIG_BLOCK_MAGIC2(pdu_utils, pdu_head_tail);

%include "pdu_utils/pdu_length_filter.h"
GR_SWIG_BLOCK_MAGIC2(pdu_utils, pdu_length_filter);
%include "pdu_utils/pdu_logger.h"
GR_SWIG_BLOCK_MAGIC2(pdu_utils, pdu_logger);
%include "pdu_utils/pdu_clock_recovery.h"
GR_SWIG_BLOCK_MAGIC2(pdu_utils, pdu_clock_recovery);
%include "pdu_utils/pdu_align.h"
GR_SWIG_BLOCK_MAGIC2(pdu_utils, pdu_align);
%include "pdu_utils/pdu_range_filter.h"
GR_SWIG_BLOCK_MAGIC2(pdu_utils, pdu_range_filter);
%include "pdu_utils/pdu_round_robin.h"
GR_SWIG_BLOCK_MAGIC2(pdu_utils, pdu_round_robin);

%include "pdu_utils/pdu_flow_ctrl_helper.h"
%include "pdu_utils/pdu_binary_tools.h"
GR_SWIG_BLOCK_MAGIC2(pdu_utils, pdu_binary_tools);

%include "pdu_utils/pdu_downsample.h"
GR_SWIG_BLOCK_MAGIC2(pdu_utils, pdu_downsample);

%include "pdu_utils/pdu_fine_time_measure.h"
GR_SWIG_BLOCK_MAGIC2(pdu_utils, pdu_fine_time_measure);

%include "pdu_utils/pdu_complex_to_mag2.h"
GR_SWIG_BLOCK_MAGIC2(pdu_utils, pdu_complex_to_mag2);

%include "pdu_utils/sandia_message_debug.h"
GR_SWIG_BLOCK_MAGIC2(pdu_utils, sandia_message_debug);
%include "pdu_utils/pdu_quadrature_demod_cf.h"
GR_SWIG_BLOCK_MAGIC2(pdu_utils, pdu_quadrature_demod_cf);
