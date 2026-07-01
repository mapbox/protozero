#ifndef PROTOZERO_CONFIG_HPP
#define PROTOZERO_CONFIG_HPP

/*****************************************************************************

protozero - Minimalistic protocol buffer decoder and encoder in C++.

This file is from https://github.com/mapbox/protozero where you can find more
documentation.

*****************************************************************************/

#include <cassert>

/**
 * @file config.hpp
 *
 * @brief Contains macro checks for different configurations.
 */

#define PROTOZERO_LITTLE_ENDIAN 1234
#define PROTOZERO_BIG_ENDIAN    4321

// Find out which byte order the machine has.

// __BYTE_ORDER__ is set by GCC and Clang.
#if !defined(PROTOZERO_BYTE_ORDER) && defined(__BYTE_ORDER__)
# if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#  define PROTOZERO_BYTE_ORDER PROTOZERO_LITTLE_ENDIAN
# elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#  define PROTOZERO_BYTE_ORDER PROTOZERO_BIG_ENDIAN
# endif
#endif // defined(__BYTE_ORDER__)

// On Linux, we can use endian.h.
#if !defined(PROTOZERO_BYTE_ORDER) && defined(__linux__)
# include <endian.h>
# if defined(__BYTE_ORDER)
#  if (__BYTE_ORDER == __LITTLE_ENDIAN)
#   define PROTOZERO_BYTE_ORDER PROTOZERO_LITTLE_ENDIAN
#  elif (__BYTE_ORDER == __BIG_ENDIAN)
#   define PROTOZERO_BYTE_ORDER PROTOZERO_BIG_ENDIAN
#  endif
# endif
#endif // !defined(PROTOZERO_BYTE_ORDER) && defined(__linux__)

// On BSD, we can use <sys/endian.h>
#if !defined(PROTOZERO_BYTE_ORDER) && \
    (defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__))
# include <sys/endian.h>
# if defined(BYTE_ORDER)
#  if (BYTE_ORDER == LITTLE_ENDIAN)
#   define PROTOZERO_BYTE_ORDER PROTOZERO_LITTLE_ENDIAN
#  elif (BYTE_ORDER == BIG_ENDIAN)
#   define PROTOZERO_BYTE_ORDER PROTOZERO_BIG_ENDIAN
#  endif
# endif
#endif

// On Windows, we assume little endian.
#if !defined(PROTOZERO_BYTE_ORDER) && defined(_MSC_VER)
# define PROTOZERO_BYTE_ORDER PROTOZERO_LITTLE_ENDIAN
#endif // !defined(PROTOZERO_BYTE_ORDER) && defined(_MSC_VER)

#if !defined(PROTOZERO_BYTE_ORDER)
# error "Could not determine byte order; define PROTOZERO_BYTE_ORDER"
#endif

// Check whether __builtin_bswap is available
#if defined(__GNUC__) || defined(__clang__)
# define PROTOZERO_USE_BUILTIN_BSWAP
#endif

// Wrapper for assert() used for testing
#ifndef protozero_assert
# define protozero_assert(x) assert(x)
#endif

#endif // PROTOZERO_CONFIG_HPP
