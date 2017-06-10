#ifndef PROTOZERO_PROTOZERO_HPP
#define PROTOZERO_PROTOZERO_HPP

/*****************************************************************************

protozero - Minimalistic protocol buffer decoder and encoder in C++.

This file is from https://github.com/mapbox/protozero where you can find more
documentation.

*****************************************************************************/

/**
 * @file protozero.hpp
 *
 * @brief Includes all public protozero headers.
 */

#if PROTOZERO_BYTE_ORDER != PROTOZERO_LITTLE_ENDIAN
  #include <protozero/byteswap.hpp>
#endif

#include <protozero/config.hpp>
#include <protozero/exception.hpp>
#include <protozero/iterators.hpp>
#include <protozero/pbf_builder.hpp>
#include <protozero/pbf_message.hpp>
#include <protozero/pbf_reader.hpp>
#include <protozero/pbf_writer.hpp>
#include <protozero/types.hpp>
#include <protozero/varint.hpp>
#include <protozero/version.hpp>

#endif // PROTOZERO_PROTOZERO_HPP
