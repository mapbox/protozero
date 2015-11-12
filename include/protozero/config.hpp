#ifndef PROTOZERO_CONFIG_HPP
#define PROTOZERO_CONFIG_HPP

/*****************************************************************************

protozero - Minimalistic protocol buffer decoder and encoder in C++.

This file is from https://github.com/mapbox/protozero where you can find more
documentation.

*****************************************************************************/

/**
 * @file config.hpp
 *
 * @brief Contains macro checks for different configurations.
 */

// Find out which byte order the machine has.
#if defined(__BYTE_ORDER)
# if (__BYTE_ORDER == __LITTLE_ENDIAN)
#  define PROTOZERO_BYTE_ORDER LITTLE_ENDIAN
# endif
# if (__BYTE_ORDER == __BIG_ENDIAN)
#  define PROTOZERO_BYTE_ORDER BIG_ENDIAN
# endif
#endif

// On some ARM machines and depending on compiler settings access to unaligned
// floating point values will result in a SIGBUS. Do not use the bare pointers
// in this case.
#if PROTOZERO_BYTE_ORDER == LITTLE_ENDIAN
# if !defined(__arm__) && !defined(_M_ARM)
#  define PROTOZERO_USE_BARE_POINTER_FOR_PACKED_FIXED
# endif
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
