#ifndef PROTOZERO_BUFFER_STRING_HPP
#define PROTOZERO_BUFFER_STRING_HPP

/*****************************************************************************

protozero - Minimalistic protocol buffer decoder and encoder in C++.

This file is from https://github.com/mapbox/protozero where you can find more
documentation.

*****************************************************************************/

/**
 * @file buffer_string.hpp
 *
 * @brief Contains the customization points for buffer implementation based
 *        on std::string
 */

#include <protozero/buffer.hpp>

#include <cstddef>
#include <iterator>
#include <string>

namespace protozero {

// Implementation of buffer customizations points for std::string

/// @cond INTERNAL
template <>
inline void buffer_append_zeros<std::string>(std::string* buffer, std::size_t count) {
    buffer->append(count, '\0');
}

template <>
inline void buffer_reserve_additional<std::string>(std::string* buffer, std::size_t size) {
    buffer->reserve(buffer->size() + size);
}

template <>
inline void buffer_erase_range<std::string>(std::string* buffer, std::size_t from, std::size_t to) {
    protozero_assert(from <= buffer->size());
    protozero_assert(to <= buffer->size());
    protozero_assert(from <= to);
    buffer->erase(std::next(buffer->begin(), from), std::next(buffer->begin(), to));
}

template <>
inline char* buffer_at_pos<std::string>(std::string* buffer, std::size_t pos) {
    protozero_assert(pos <= buffer->size());
    return (&*buffer->begin()) + pos;
}
/// @endcond

} // namespace protozero

#endif // PROTOZERO_BUFFER_STRING_HPP
