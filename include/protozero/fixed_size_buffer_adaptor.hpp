#ifndef PROTOZERO_FIXED_SIZE_BUFFER_ADAPTOR_HPP
#define PROTOZERO_FIXED_SIZE_BUFFER_ADAPTOR_HPP

/*****************************************************************************

protozero - Minimalistic protocol buffer decoder and encoder in C++.

This file is from https://github.com/mapbox/protozero where you can find more
documentation.

*****************************************************************************/

/**
 * @file fixed_size_buffer_adaptor.hpp
 *
 * @brief Contains the fixed_size_buffer_adaptor class.
 */

#include <protozero/buffer_tmpl.hpp>
#include <protozero/config.hpp>

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <stdexcept>

namespace protozero {

/**
 * This class can be used instead of std::string if you want to create a
 * vector tile in a fixed-size buffer. Any operation that needs more space
 * than is available will fail with a std::length_error exception.
 *
 * As user of this class you should not rely on anything here beyond the
 * constructor, the use of everything else is for the protozero library
 * only.
 */
class fixed_size_buffer_adaptor {

    char* m_data;
    std::size_t m_capacity;
    std::size_t m_size;

public:

    /**
     * Constructor.
     *
     * @param data Pointer to some memory allocated for the buffer.
     * @param size Number of bytes available.
     */
    fixed_size_buffer_adaptor(char* data, std::size_t size) :
        m_data(data),
        m_capacity(size),
        m_size(0) {
    }

    /**
     * Constructor.
     *
     * @param container Some container class supporting the member functions
     *        data() and size().
     */
    template <typename T>
    explicit fixed_size_buffer_adaptor(T& container) :
        m_data(container.data()),
        m_capacity(container.size()),
        m_size(0) {
    }

/// @cond INTERNAL
    const char* data() const noexcept {
        return m_data;
    }

    std::size_t capacity() const noexcept {
        return m_capacity;
    }

    std::size_t size() const noexcept {
        return m_size;
    }

    void append(const char* data, std::size_t count) {
        if (m_size + count > m_capacity) {
            throw std::length_error{"fixed size data store exhausted"};
        }
        std::copy_n(data, count, m_data + m_size);
        m_size += count;
    }

    void append_zeros(std::size_t count) {
        if (m_size + count > m_capacity) {
            throw std::length_error{"fixed size data store exhausted"};
        }
        std::fill_n(m_data + m_size, count, '\0');
        m_size += count;
    }

    void resize(std::size_t size) {
        protozero_assert(size < m_size);
        if (size > m_capacity) {
            throw std::length_error{"fixed size data store exhausted"};
        }
        m_size = size;
    }

    void erase_range(std::size_t from, std::size_t to) {
        protozero_assert(from <= m_size);
        protozero_assert(to <= m_size);
        protozero_assert(from < to);
        std::copy(m_data + to, m_data + m_size, m_data + from);
        m_size -= (to - from);
    }

    char* at_pos(std::size_t pos) {
        protozero_assert(pos <= m_size);
        return m_data + pos;
    }

    void push_back(char ch) {
        if (m_size >= m_capacity) {
            throw std::length_error{"fixed size data store exhausted"};
        }
        m_data[m_size++] = ch;
    }
/// @endcond

}; // class fixed_size_buffer_adaptor

/// @cond INTERNAL
template <>
struct buffer_customization<fixed_size_buffer_adaptor> {

    static std::size_t size(const fixed_size_buffer_adaptor* buffer) noexcept {
        return buffer->size();
    }

    static void append(fixed_size_buffer_adaptor* buffer, const char* data, std::size_t count) {
        buffer->append(data, count);
    }

    static void append_zeros(fixed_size_buffer_adaptor* buffer, std::size_t count) {
        buffer->append_zeros(count);
    }

    static void resize(fixed_size_buffer_adaptor* buffer, std::size_t size) {
        buffer->resize(size);
    }

    static void reserve_additional(fixed_size_buffer_adaptor* /*buffer*/, std::size_t /*size*/) {
        /* nothing to be done for fixed-size buffers */
    }

    static void erase_range(fixed_size_buffer_adaptor* buffer, std::size_t from, std::size_t to) {
        buffer->erase_range(from, to);
    }

    static char* at_pos(fixed_size_buffer_adaptor* buffer, std::size_t pos) {
        return buffer->at_pos(pos);
    }

    static void push_back(fixed_size_buffer_adaptor* buffer, char ch) {
        buffer->push_back(ch);
    }

};
/// @endcond

} // namespace protozero

#endif // PROTOZERO_FIXED_SIZE_BUFFER_ADAPTOR_HPP
