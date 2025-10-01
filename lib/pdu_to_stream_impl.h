/* -*- c++ -*- */
/*
 * Copyright 2025 Sam Wolfson.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_PLASMA_PDU_TO_STREAM_IMPL_H
#define INCLUDED_PLASMA_PDU_TO_STREAM_IMPL_H

#include <gnuradio/plasma/pdu_to_stream.h>

namespace gr {
  namespace plasma {

    class pdu_to_stream_impl : public pdu_to_stream
    {
     private:
      // Nothing to declare in this block.

     public:
      pdu_to_stream_impl();
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
