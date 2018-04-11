/* -*- c++ -*- */
/*
 * Copyright 2018 National Technology & Engineering Solutions of Sandia, LLC (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government retains certain rights in this software.
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */


 /* @WARNING@ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "@IMPL_NAME@.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>

namespace gr {
  namespace pdu_utils {

    @BASE_NAME@::sptr
    @BASE_NAME@::make(uint32_t early_burst_behavior, uint32_t max_queue_size)
    {
      return gnuradio::get_initial_sptr
        (new @IMPL_NAME@(early_burst_behavior, max_queue_size));
    }

    /* BEHAVIOR OF SUCCESSIVE BURSTS

      -- APPEND MODE --
        If an untimed message is received with data still queued from previous
        messages, it will be appended to the end of the same burst.
        If a timed message is received it will be queued and transmitted with
        its own tx_sob and tx_eob tags. This will potentially cause late bursts
        to be ignored downstream at the USRP sink block.

      -- DROP MODE --
        If messages are received in rapid succession, any that arrive while
        there is still data queued from previous PMTs will be dropped.

      -- BALK MODE --
        Same behavior as drop mode except a warning will also be emitted

        #TODO   add an option such that if a burst will be late, change it to
                untimed mode to prevent it from being dropped by UHD
    */


    /*
     * The private constructor
     */
    @IMPL_NAME@::@IMPL_NAME@(uint32_t early_burst_behavior, uint32_t max_queue_size)
      : gr::sync_block("@NAME@",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(1, 1, sizeof(@O_TYPE@))),
        d_itemsize(sizeof(@O_TYPE@)),
        d_max_queue_size(64),
        d_time_tag(pmt::PMT_NIL)
    {
      d_data.clear();
      d_pdu_queue.clear();

      if (early_burst_behavior == EARLY_BURST_BEHAVIOR__APPEND) {
        d_drop_early_bursts = false;
        d_early_burst_err = false;
      } else if (early_burst_behavior == EARLY_BURST_BEHAVIOR__DROP) {
        d_drop_early_bursts = true;
        d_early_burst_err = false;
      } else {  // balk
        d_drop_early_bursts = true;
        d_early_burst_err = true;
      }

      message_port_register_in(PMTCONSTSTR__BURSTS);
      set_msg_handler(PMTCONSTSTR__BURSTS,
          boost::bind(&@IMPL_NAME@::store_pdu, this, _1));
    }

    /*
     * Our virtual destructor.
     */
    @IMPL_NAME@::~@IMPL_NAME@()
    {
    }


    /*
     * function validates PDUs and stores them in a queue for further processing
     */
    void
    @IMPL_NAME@::store_pdu(pmt::pmt_t pdu)
    {
      // check and see if there is already data in the vector, drop if in drop mode
      if (d_drop_early_bursts & (d_data.size() | d_pdu_queue.size())) {
        if (d_early_burst_err) {
          GR_LOG_ERROR(d_logger, "PDU received before previous burst finished writing - dropped");
        }
        return;
      }

      // make sure PDU data is formed properly
      if (!(pmt::is_pair(pdu))) {
        GR_LOG_ERROR(d_logger, "received unexpected PMT (non-pair)");
        return;
      }

      pmt::pmt_t meta = pmt::car(pdu);
      pmt::pmt_t v_data = pmt::cdr(pdu);

      if (!(is_dict(meta) && pmt::is_uniform_vector(v_data))) {
        GR_LOG_ERROR(d_logger, "PMT is not a PDU, dropping");
        return;
      }

      if (pmt::length(v_data) != 0) {
        size_t v_itemsize = pmt::uniform_vector_itemsize(v_data);
        if (v_itemsize != d_itemsize) {
          GR_LOG_ERROR(d_logger, boost::format("PDU received has incorrect itemsize (%d != %d)") % v_itemsize % d_itemsize);
          return;
        }

        // pdu data is valid and nonzero length, queue it
        if (d_pdu_queue.size() < d_max_queue_size) {
          d_pdu_queue.push_back(pdu);
          d_drop_ctr = 0;
        } else {
          d_drop_ctr++;
          GR_LOG_WARN(d_logger, boost::format("Queue full, PDU dropped (%d dropped so far)") % d_drop_ctr);
        }
        //std::cout << "Now there are " << d_pdu_queue.size() << " items in the PDU queue" << std::endl;
      } else {
        GR_LOG_WARN(d_logger, "zero size PDU ignored");
      }

      return;
    }


    /*
     * this function will pop PDUs off the queue and store the data, returns
     * the number of data elements queued
     */
    uint32_t
    @IMPL_NAME@::queue_data()
    {
      uint32_t data_size = 0;
      // this should only get called when there is data in the queue, but check
      // anyway and return if it is empty
      if (d_pdu_queue.empty()) {
        return data_size;
      }

      pmt::pmt_t pdu = d_pdu_queue.front();
      d_pdu_queue.pop_front();

      // only validated PDUs allowed into queue, validation not required here
      pmt::pmt_t meta = pmt::car(pdu);
      pmt::pmt_t v_data = pmt::cdr(pdu);

      // retrieve the data
      const std::vector<@O_TYPE@> d_in = pmt::@PMT_TYPE@vector_elements(v_data);

      // add the data to the queue
      d_data.insert(d_data.end(), d_in.begin(), d_in.end());
      data_size += d_in.size();

      // check if there are future PDUs queued that are untimed and queue them also
      bool done = false;
      while (!done) {
        if (d_pdu_queue.empty()) {
          done = true;
          break;
        }
        pmt::pmt_t pdu2 = d_pdu_queue.front();
        pmt::pmt_t meta2 = pmt::car(pdu2);
        if (pmt::dict_has_key(meta2, PMTCONSTSTR__TX_TIME)) {
          // the next PDU is timed, so we will handle that on the next call
          done = true;
        } else {
          d_pdu_queue.pop_front();
          v_data = pmt::cdr(pdu2);
          const std::vector<@O_TYPE@> d_in2 = pmt::@PMT_TYPE@vector_elements(v_data);
          d_data.insert(d_data.end(), d_in2.begin(), d_in2.end());
          data_size += d_in2.size();
        }
      }

      d_tag_sob = true;

      // if the burst is timed, calculate the time tag
      pmt::pmt_t pmt_time = pmt::dict_ref(meta, PMTCONSTSTR__TX_TIME, pmt::PMT_NIL);
      // TODO: is this level of checking warranted? what is the impact?
      if (pmt::is_tuple(pmt_time) && pmt::length(pmt_time) >= 2 &&
          pmt::is_uint64(pmt::tuple_ref(pmt_time, 0)) &&
          pmt::is_real(pmt::tuple_ref(pmt_time, 1))) {
        // it's a good tuple...
        d_time_tag = pmt_time;
      }

      return data_size;
    }


    int
    @IMPL_NAME@::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      @O_TYPE@ *out = (@O_TYPE@ *) output_items[0];

      uint32_t produced = 0;
      uint32_t data_remaining = d_data.size();

      // if there are no data in the queue, see if more PDUs are ready
      if (data_remaining == 0) {
        if (d_pdu_queue.empty()) {
          // if we have nothing to do, sleep for a short duration to prevent rapid
          // successive calls and then return zero items
          boost::this_thread::sleep(boost::posix_time::microseconds(25));
          return 0;
        }

        // fetch another PDU of data and update the size of the data
        data_remaining = queue_data();
        if (data_remaining == 0) {
          boost::this_thread::sleep(boost::posix_time::microseconds(25));
          return 0;
        }

      // otherwise, we will be generating some data
      } else {
        if (d_tag_sob) {
          add_item_tag(0, nitems_written(0), PMTCONSTSTR__TX_SOB, pmt::PMT_T);
          //std::cout << "tagging SOB on sample " << (nitems_written(0)) << std::endl;
          d_tag_sob = false;

          // if there is a time tag waiting, use it then reset the time tag
          if (!pmt::eqv(d_time_tag, pmt::PMT_NIL)) {
            add_item_tag(0, nitems_written(0), PMTCONSTSTR__TX_TIME, d_time_tag);
            d_time_tag = pmt::PMT_NIL;
          }
        }

        // if everything remaining will fit, send it and tag EOB
        if (data_remaining <= noutput_items) {
          //std::cout << "writing " << data_remaining << " items (done)" << std::endl;

          memcpy(out, &d_data[0], d_itemsize*data_remaining);
          // tag last item "tx_eob, True"
          add_item_tag(0, nitems_written(0) + data_remaining - 1,  PMTCONSTSTR__TX_EOB, pmt::PMT_T);
          //std::cout << "tagging EOB on sample " << (nitems_written(0) + data_remaining - 1) << std::endl;
          d_data.clear();
          produced = data_remaining;

        // not everything will fit... send first noutput_items from PDU
        } else {
          if (noutput_items) {
            //std::cout << "writing " << noutput_items << " items" << std::endl;

            memcpy(out, &d_data[0], d_itemsize*noutput_items);
            d_data.erase(d_data.begin(), d_data.begin() + noutput_items - 1);
            produced = noutput_items;
          }
        }
      }

      // Tell runtime system how many output items we produced.
      return produced;
    }

  } /* namespace pdu_utils */
} /* namespace gr */
