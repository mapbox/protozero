#ifndef PROTOZERO_ITERATORS_HPP
#define PROTOZERO_ITERATORS_HPP

/*****************************************************************************

protozero - Minimalistic protocol buffer decoder and encoder in C++.

This file is from https://github.com/mapbox/protozero where you can find more
documentation.

*****************************************************************************/

#include <iterator>

#include <protozero/byteswap.hpp>
#include <protozero/varint.hpp>

namespace protozero {

#ifdef PROTOZERO_USE_BARE_POINTER_FOR_PACKED_FIXED

    template <typename T>
    using const_fixed_iterator = const T*;

    template <typename T>
    inline std::pair<const_fixed_iterator<T>, const_fixed_iterator<T>> create_fixed_iterator_pair(const char* first, const char* last) {
        return std::make_pair(reinterpret_cast<const T*>(first),
                              reinterpret_cast<const T*>(last));
    }

#else

    template <typename T>
    class const_fixed_iterator {

        const char* m_data;
        const char* m_end;

    public:

        using iterator_category = std::forward_iterator_tag;
        using value_type        = T;
        using difference_type   = std::ptrdiff_t;
        using pointer           = value_type*;
        using reference         = value_type&;

        const_fixed_iterator() noexcept :
            m_data(nullptr),
            m_end(nullptr) {
        }

        const_fixed_iterator(const char* data, const char* end) noexcept :
            m_data(data),
            m_end(end) {
        }

        const_fixed_iterator(const const_fixed_iterator&) noexcept = default;
        const_fixed_iterator(const_fixed_iterator&&) noexcept = default;

        const_fixed_iterator& operator=(const const_fixed_iterator&) noexcept = default;
        const_fixed_iterator& operator=(const_fixed_iterator&&) noexcept = default;

        ~const_fixed_iterator() noexcept = default;

        value_type operator*() {
            value_type result;
            copy_or_byteswap<sizeof(value_type)>(m_data , &result);
            return result;
        }

        const_fixed_iterator& operator++() {
            m_data += sizeof(value_type);
            return *this;
        }

        const_fixed_iterator operator++(int) {
            const const_fixed_iterator tmp(*this);
            ++(*this);
            return tmp;
        }

        bool operator==(const const_fixed_iterator& rhs) const noexcept {
            return m_data == rhs.m_data && m_end == rhs.m_end;
        }

        bool operator!=(const const_fixed_iterator& rhs) const noexcept {
            return !(*this == rhs);
        }

    }; // class const_fixed_iterator

    template <typename T>
    inline std::pair<const_fixed_iterator<T>, const_fixed_iterator<T>> create_fixed_iterator_pair(const char* first, const char* last) {
        return std::make_pair(const_fixed_iterator<T>(first, last),
                              const_fixed_iterator<T>(last, last));
    }

#endif

    template <typename T>
    class const_varint_iterator {

    protected:

        const char* m_data;
        const char* m_end;

    public:

        using iterator_category = std::forward_iterator_tag;
        using value_type        = T;
        using difference_type   = std::ptrdiff_t;
        using pointer           = value_type*;
        using reference         = value_type&;

        const_varint_iterator() noexcept :
            m_data(nullptr),
            m_end(nullptr) {
        }

        const_varint_iterator(const char* data, const char* end) noexcept :
            m_data(data),
            m_end(end) {
        }

        const_varint_iterator(const const_varint_iterator&) noexcept = default;
        const_varint_iterator(const_varint_iterator&&) noexcept = default;

        const_varint_iterator& operator=(const const_varint_iterator&) noexcept = default;
        const_varint_iterator& operator=(const_varint_iterator&&) noexcept = default;

        ~const_varint_iterator() noexcept = default;

        value_type operator*() {
            const char* d = m_data; // will be thrown away
            return static_cast<value_type>(decode_varint(&d, m_end));
        }

        const_varint_iterator& operator++() {
            // Ignore the result, we call decode_varint() just for the
            // side-effect of updating m_data.
            decode_varint(&m_data, m_end);
            return *this;
        }

        const_varint_iterator operator++(int) {
            const const_varint_iterator tmp(*this);
            ++(*this);
            return tmp;
        }

        bool operator==(const const_varint_iterator& rhs) const noexcept {
            return m_data == rhs.m_data && m_end == rhs.m_end;
        }

        bool operator!=(const const_varint_iterator& rhs) const noexcept {
            return !(*this == rhs);
        }

    }; // class const_varint_iterator

    template <typename T>
    class const_svarint_iterator : public const_varint_iterator<T> {

    public:

        using iterator_category = std::forward_iterator_tag;
        using value_type        = T;
        using difference_type   = std::ptrdiff_t;
        using pointer           = value_type*;
        using reference         = value_type&;

        const_svarint_iterator() noexcept :
            const_varint_iterator<T>() {
        }

        const_svarint_iterator(const char* data, const char* end) noexcept :
            const_varint_iterator<T>(data, end) {
        }

        const_svarint_iterator(const const_svarint_iterator&) = default;
        const_svarint_iterator(const_svarint_iterator&&) = default;

        const_svarint_iterator& operator=(const const_svarint_iterator&) = default;
        const_svarint_iterator& operator=(const_svarint_iterator&&) = default;

        ~const_svarint_iterator() = default;

        value_type operator*() {
            const char* d = this->m_data; // will be thrown away
            return static_cast<value_type>(decode_zigzag64(decode_varint(&d, this->m_end)));
        }

        const_svarint_iterator& operator++() {
            // Ignore the result, we call decode_varint() just for the
            // side-effect of updating m_data.
            decode_varint(&this->m_data, this->m_end);
            return *this;
        }

        const_svarint_iterator operator++(int) {
            const const_svarint_iterator tmp(*this);
            ++(*this);
            return tmp;
        }

    }; // class const_svarint_iterator

} // end namespace protozero

#endif // PROTOZERO_ITERATORS_HPP
