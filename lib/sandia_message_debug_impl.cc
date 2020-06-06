/* -*- c++ -*- */
/* 
 * Copyright 2019 gr-pdu_utils author.
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "sandia_message_debug_impl.h"

namespace gr
{
  namespace pdu_utils
  {

    sandia_message_debug::sptr sandia_message_debug::make()
    {
      return gnuradio::get_initial_sptr( new sandia_message_debug_impl() );
    }

    /*
     * The private constructor
     */
    sandia_message_debug_impl::sandia_message_debug_impl() : gr::block( "sandia_message_debug",
        gr::io_signature::make( 0, 0, 0 ), gr::io_signature::make( 0, 0, 0 ) )
    {
      message_port_register_in( pmt::mp( "print" ) );
      set_msg_handler( pmt::mp( "print" ), boost::bind( &sandia_message_debug_impl::print, this, _1 ) );

      message_port_register_in( pmt::mp( "store" ) );
      set_msg_handler( pmt::mp( "store" ), boost::bind( &sandia_message_debug_impl::store, this, _1 ) );

      message_port_register_in( pmt::mp( "print_pdu" ) );
      set_msg_handler( pmt::mp( "print_pdu" ), boost::bind( &sandia_message_debug_impl::print_pdu, this, _1 ) );
    }

    /*
     * Our virtual destructor.
     */
    sandia_message_debug_impl::~sandia_message_debug_impl()
    {
      return;
    }

    /**
     * prints msg to STDOUT using pmt::write_string()
     *
     * @param msg - message to print
     */
    void sandia_message_debug_impl::print( pmt::pmt_t msg )
    {
      std::stringstream sout;
      sout << "************* MESSAGE DEBUG++ PRINT ***************\n";
      sout << pmt::write_string( msg ) << std::endl;
      sout << "***************************************************\n";
      std::cout << sout.str();
    }

    /**
     * Stores msg in internal vector for later retrival
     *
     * @param msg - message to store
     */
    void sandia_message_debug_impl::store( pmt::pmt_t msg )
    {
      gr::thread::scoped_lock guard( d_mutex );
      d_messages.push_back( msg );
    }

    /**
     * Prints a PMT message to STDOUT
     * Assumes incoming pmt is a PMT pair of meta data and u8vector data
     *
     * @param pdu - incoming PDU to print
     */
    void sandia_message_debug_impl::print_pdu( pmt::pmt_t pdu )
    {
      std::stringstream sout;

      if( pmt::is_null(pdu) )
      {
        return;
      }
      if( !pmt::is_pair( pdu ) )
      {
        return;
      }


      pmt::pmt_t meta = pmt::car( pdu );
      pmt::pmt_t vector = pmt::cdr( pdu );

      sout << "********* MESSAGE DEBUG++ PRINT PDU VERBOSE *********\n";
      sout << pmt::write_string( meta );

      if( pmt::is_uniform_vector( vector ) )
      {

        size_t len = pmt::blob_length( vector );
        sout << "pdu_length = " << len << std::endl;
        sout << "contents = " << std::endl;
        size_t offset( 0 );
        const uint8_t *d = (const uint8_t*)pmt::uniform_vector_elements( vector, offset );

        //loop over 16 byte blocks
        for( size_t i = 0; i < len; i += 16 )
        {
          //print address offset
          sout << boost::format( "%04x: " ) % (unsigned int)i;

          //loop over individual 16 bytes
          for( size_t j = i; j < std::min( i + 16, len ); j++ )
          {
            sout << boost::format( "%02x " ) % (unsigned int)d[j];
          }

          sout << std::endl;
        }

      } //end if( is_uniform_vector
      else
      {
        sout << "No vector data\n";
      }

      sout << "***************************************************\n";
      std::cout << sout.str();
    }

    /**
     * Returns the number of messages stored
     *
     * @return int - number of messages stored
     */
    int sandia_message_debug_impl::num_messages()
    {
      return (int)d_messages.size();
    }

    /**
     * Returns a specific stored message
     *
     * @param i - message index
     * @return pmt::pmt_t - stored message
     */
    pmt::pmt_t sandia_message_debug_impl::get_message( int i )
    {
      gr::thread::scoped_lock guard( d_mutex );

      if( (size_t)i >= d_messages.size() )
      {
        throw std::runtime_error( "message_debug: index for message out of bounds.\n" );
      }

      return d_messages[i];
    }

  } /* namespace pdu_utils */
} /* namespace gr */

