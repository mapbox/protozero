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

#include <cstddef>
#include <iterator>
#include <string>

namespace protozero {

// Implementation of buffer customizations points for std::string

/// @cond INTERNAL
inline std::size_t buffer_size(const std::string* buffer) noexcept {
    return buffer->size();
}

inline void buffer_append(std::string* buffer, const char* data, std::size_t count) {
    buffer->append(data, count);
}

inline void buffer_append_zeros(std::string* buffer, std::size_t count) {
    buffer->append(count, '\0');
}

inline void buffer_resize(std::string* buffer, std::size_t size) {
    protozero_assert(size < buffer_size(buffer));
    buffer->resize(size);
}

inline void buffer_reserve_additional(std::string* buffer, std::size_t size) {
    buffer->reserve(buffer->size() + size);
}

inline void buffer_erase_range(std::string* buffer, std::size_t from, std::size_t to) {
    protozero_assert(from <= buffer->size());
    protozero_assert(to <= buffer->size());
    protozero_assert(from <= to);
    buffer->erase(std::next(buffer->begin(), from), std::next(buffer->begin(), to));
}

inline char* buffer_at_pos(std::string* buffer, std::size_t pos) {
    protozero_assert(pos <= buffer->size());
    return (&*buffer->begin()) + pos;
}
/// @endcond

} // namespace protozero

#endif // PROTOZERO_BUFFER_STRING_HPP
