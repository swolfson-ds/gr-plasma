/* -*- c++ -*- */
/*
 * Copyright 2025 Sam Wolfson.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <gnuradio/io_signature.h>
#include "pdu_to_stream_impl.h"
#include <cstring>

namespace gr {
  namespace plasma {

    using output_type = float;
    pdu_to_stream::sptr
    pdu_to_stream::make()
    {
      return gnuradio::make_block_sptr<pdu_to_stream_impl>(
        );
    }


    /*
     * The private constructor
     */
    pdu_to_stream_impl::pdu_to_stream_impl(bool repeat)
      : gr::sync_block("pdu_to_stream",
              gr::io_signature::make(0,0,0),
              gr::io_signature::make(1, 1, sizeof(gr_complex))),
        d_sample_index(0),
        d_repeat(repeat)
    {
        message_port_register_in(pmt::mp("pdus"));
        set_msg_handler(pmt::mp("pdus"), [this](pmt::pmt_t msg) { this->handle_pdu(msg); });
        GR_LOG_INFO(d_logger, "PDU to Stream initialized (repeat=" + 
                  std::string(d_repeat ? "true" : "false") + ")");
    }

    /*
     * Our virtual destructor.
     */
    pdu_to_stream_impl::~pdu_to_stream_impl()
    {
    }


    void
    pdu_to_stream_impl::handle_pdu(pmt::pmt_t msg)
    {
      // PDU format: pair of (metadata_dict, uniform_vector)
      if (!pmt::is_pair(msg)) {
        GR_LOG_WARN(d_logger, "Received non-pair PDU, ignoring");
        return;
      }
      
      pmt::pmt_t meta = pmt::car(msg);
      pmt::pmt_t vector = pmt::cdr(msg);
      
      // Check if it's a complex vector
      if (!pmt::is_c32vector(vector)) {
        GR_LOG_WARN(d_logger, "Received non-complex PDU, ignoring");
        return;
      }
      
      // Get the complex samples
      size_t len;
      const gr_complex* samples = pmt::c32vector_elements(vector, len);
      
      if (len == 0) {
        GR_LOG_WARN(d_logger, "Received empty PDU");
        return;
      }
      
      // Store the waveform (replaces previous waveform)
      std::lock_guard<std::mutex> lock(d_mutex);
      d_waveform.clear();
      d_waveform.reserve(len);
      for (size_t i = 0; i < len; i++) {
        d_waveform.push_back(samples[i]);
      }
      d_sample_index = 0;  // Reset playback position
      
      GR_LOG_INFO(d_logger, "Received PDU with " + std::to_string(len) + " samples");
      
      // Optional: Log metadata if present
      if (pmt::is_dict(meta)) {
        pmt::pmt_t keys = pmt::dict_keys(meta);
        size_t nkeys = pmt::length(keys);
        if (nkeys > 0) {
          GR_LOG_DEBUG(d_logger, "PDU metadata contains " + std::to_string(nkeys) + " keys");
        }
      }
    }

    int
    pdu_to_stream_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
        gr_complex *out = static_cast<gr_complex *>(output_items[0]);
        std::lock_guard<std::mutex> lock(d_mutex);
        if (d_waveform.empty()) {
            // No waveform loaded, output zeros
            std::memset(out, 0, noutput_items * sizeof(gr_complex));
            return noutput_items;
        }

        if (d_repeat) {
            for (int i = 0; i < noutput_items; i++) {
                out[i] = d_waveform[d_sample_index];
                d_sample_index = (d_sample_index + 1) % d_waveform.size();
            }
            return noutput_items;
        } else {
            for (int i = 0; i < noutput_items; i++) {
                if (d_sample_index < d_waveform.size()) {
                    out[i] = d_waveform[d_sample_index];
                    d_sample_index++;                    
                } else {
                    out[i] = gr_complex(0, 0); // Pad with zeros after waveform ends
                }
            }
            
        }
        return noutput_items;
    }

  } /* namespace plasma */
} /* namespace gr */
