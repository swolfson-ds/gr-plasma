/* -*- c++ -*- */
/*
 * Copyright 2025 Sam Wolfson.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_PLASMA_PDU_TO_STREAM_H
#define INCLUDED_PLASMA_PDU_TO_STREAM_H

#include <gnuradio/plasma/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace plasma {

    /*!
     * \brief <+description of block+>
     * \ingroup plasma
     *
     */
    class PLASMA_API pdu_to_stream : virtual public gr::sync_block
    {
     public:
      typedef std::shared_ptr<pdu_to_stream> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of plasma::pdu_to_stream.
       *
       * To avoid accidental use of raw pointers, plasma::pdu_to_stream's
       * constructor is in a private implementation
       * class. plasma::pdu_to_stream::make is the public interface for
       * creating new instances.
       */
      static sptr make(bool repeat = true);
    };

  } // namespace plasma
} // namespace gr

#endif /* INCLUDED_PLASMA_PDU_TO_STREAM_H */
