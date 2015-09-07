#ifndef PROTOZERO_BYTESWAP_HPP
#define PROTOZERO_BYTESWAP_HPP

/*****************************************************************************

protozero - Minimalistic protocol buffer decoder and encoder in C++.

This file is from https://github.com/mapbox/protozero where you can find more
documentation.

*****************************************************************************/

#include <cassert>

// Windows is only available for little endian architectures
// http://stackoverflow.com/questions/6449468/can-i-safely-assume-that-windows-installations-will-always-be-little-endian
#if !defined(_WIN32) && !defined(__APPLE__)
# include <endian.h>
#else
# define __LITTLE_ENDIAN 1234
# define __BYTE_ORDER __LITTLE_ENDIAN
#endif

namespace protozero {

#if __BYTE_ORDER == __LITTLE_ENDIAN

template <int N>
inline void byteswap(const char* /*data*/, char* /*result*/) {
    assert(false);
}

template <>
inline void byteswap<1>(const char* data, char* result) {
    result[0] = data[0];
}

template <>
inline void byteswap<4>(const char* data, char* result) {
    result[3] = data[0];
    result[2] = data[1];
    result[1] = data[2];
    result[0] = data[3];
}

template <>
inline void byteswap<8>(const char* data, char* result) {
    result[7] = data[0];
    result[6] = data[1];
    result[5] = data[2];
    result[4] = data[3];
    result[3] = data[4];
    result[2] = data[5];
    result[1] = data[6];
    result[0] = data[7];
}

#else // __BYTE_ORDER == __LITTLE_ENDIAN

template <int N>
inline void byteswap(const char* data, char* result) {
    memcpy(result, data, N);
}

#endif // __BYTE_ORDER == __LITTLE_ENDIAN

} // end namespace protozero

#endif // PROTOZERO_BYTESWAP_HPP
