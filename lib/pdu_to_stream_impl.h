/* -*- c++ -*- */
/*
 * Copyright 2025 Sam Wolfson.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_PLASMA_PDU_TO_STREAM_IMPL_H
#define INCLUDED_PLASMA_PDU_TO_STREAM_IMPL_H

#include <gnuradio/plasma/pdu_to_stream.h>
#include <vector>
#include <mutex>


namespace gr {
  namespace plasma {

    class pdu_to_stream_impl : public pdu_to_stream
    {
     private:
      std::vector<gr_complex> d_waveform;
      size_t d_sample_index;
      std::mutex d_mutex;
      bool d_repeat;

      void handle_pdu(pmt::pmt_t msg);

     public:
      pdu_to_stream_impl(bool repeat=true);
      ~pdu_to_stream_impl();

      // Where all the action really happens
      int work(
              int noutput_items,
              gr_vector_const_void_star &input_items,
              gr_vector_void_star &output_items
      );
    };

  } // namespace plasma
} // namespace gr

#endif /* INCLUDED_PLASMA_PDU_TO_STREAM_IMPL_H */
