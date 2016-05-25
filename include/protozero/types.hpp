#ifndef PROTOZERO_TYPES_HPP
#define PROTOZERO_TYPES_HPP

/*****************************************************************************

protozero - Minimalistic protocol buffer decoder and encoder in C++.

This file is from https://github.com/mapbox/protozero where you can find more
documentation.

*****************************************************************************/

/**
 * @file types.hpp
 *
 * @brief Contains the declaration of low-level types used in the pbf format.
 */

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>

namespace protozero {

/**
 * The type used for field tags (field numbers).
 */
using pbf_tag_type = uint32_t;

/**
 * The type used to encode type information.
 * See the table on
 *    https://developers.google.com/protocol-buffers/docs/encoding
 */
enum class pbf_wire_type : uint32_t {
    varint           = 0, // int32/64, uint32/64, sint32/64, bool, enum
    fixed64          = 1, // fixed64, sfixed64, double
    length_delimited = 2, // string, bytes, embedded messages,
                            // packed repeated fields
    fixed32          = 5, // fixed32, sfixed32, float
    unknown          = 99 // used for default setting in this library
};

/**
 * The type used for length values, such as the length of a field.
 */
using pbf_length_type = uint32_t;

#ifdef PROTOZERO_USE_VIEW
using data_view = PROTOZERO_USE_VIEW;
#else

/**
 * Holds a pointer to some data and a length.
 *
 * This class is supposed to be compatible with the std::string_view
 * that will be available in C++17.
 */
class data_view {

    const char* m_data;
    std::size_t m_size;

public:

    constexpr data_view(const char* data, std::size_t size) noexcept
        : m_data(data),
          m_size(size) {
    }

    data_view(const std::string& str) noexcept
        : m_data(str.data()),
          m_size(str.size()) {
    }

    data_view(const char* data) noexcept
        : m_data(data),
          m_size(std::strlen(data)) {
    }

    constexpr const char* data() const noexcept {
        return m_data;
    }
    constexpr std::size_t size() const noexcept {
        return m_size;
    }

    std::string to_string() const {
        return std::string{m_data, m_size};
    }

    explicit operator std::string() const {
        return std::string{m_data, m_size};
    }

}; // class data_view

#endif


} // end namespace protozero

#endif // PROTOZERO_TYPES_HPP
