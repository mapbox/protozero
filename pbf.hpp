#ifndef MAPBOX_UTIL_PBF_HPP
#define MAPBOX_UTIL_PBF_HPP

/*****************************************************************************

Minimalistic fast C++ decoder for a subset of the protocol buffer format.

This is header-only, meaning there is nothing to build. Just include this file
in your C++ application.

This file is from https://github.com/mapbox/pbf.hpp where you can find more
documentation.

*****************************************************************************/

/*
 * Some parts are from upb - a minimalist implementation of protocol buffers.
 *
 * Copyright (c) 2008-2011 Google Inc.  See LICENSE for details.
 * Author: Josh Haberman <jhaberman@gmail.com>
 */

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <exception>
#include <iterator>
#include <string>
#include <utility>

namespace mapbox { namespace util {

class pbf {

    // from https://github.com/facebook/folly/blob/master/folly/Varint.h
    static const int8_t kMaxVarintLength64 = 10;

    static uint64_t decode_varint(const char** data, const char* end) {
        const int8_t* begin = reinterpret_cast<const int8_t*>(*data);
        const int8_t* iend = reinterpret_cast<const int8_t*>(end);
        const int8_t* p = begin;
        uint64_t val = 0;

        if (iend - begin >= kMaxVarintLength64) {  // fast path
            do {
                int64_t b;
                b = *p++; val  = static_cast<uint64_t>((b & 0x7f)      ); if (b >= 0) break;
                b = *p++; val |= static_cast<uint64_t>((b & 0x7f) <<  7); if (b >= 0) break;
                b = *p++; val |= static_cast<uint64_t>((b & 0x7f) << 14); if (b >= 0) break;
                b = *p++; val |= static_cast<uint64_t>((b & 0x7f) << 21); if (b >= 0) break;
                b = *p++; val |= static_cast<uint64_t>((b & 0x7f) << 28); if (b >= 0) break;
                b = *p++; val |= static_cast<uint64_t>((b & 0x7f) << 35); if (b >= 0) break;
                b = *p++; val |= static_cast<uint64_t>((b & 0x7f) << 42); if (b >= 0) break;
                b = *p++; val |= static_cast<uint64_t>((b & 0x7f) << 49); if (b >= 0) break;
                b = *p++; val |= static_cast<uint64_t>((b & 0x7f) << 56); if (b >= 0) break;
                b = *p++; val |= static_cast<uint64_t>((b & 0x7f) << 63); if (b >= 0) break;
                throw varint_too_long_exception();
            } while (false);
        } else {
            int shift = 0;
            while (p != iend && *p < 0) {
                val |= static_cast<uint64_t>(*p++ & 0x7f) << shift;
                shift += 7;
            }
            if (p == iend) {
                throw end_of_buffer_exception();
            }
            val |= static_cast<uint64_t>(*p++) << shift;
        }

        *data = reinterpret_cast<const char*>(p);
        return val;
    }

    const char *m_data = nullptr;
    const char *m_end = nullptr;
    uint32_t m_value = 0;
    uint32_t m_tag = 0;

    template <typename T> inline T fixed();
    template <typename T> inline T varint();
    template <typename T> inline T svarint();
    template <typename T> inline std::pair<const T*, const T*> packed_fixed();

    inline void skip_bytes(uint32_t len);

    inline uint32_t get_len_and_skip();

public:

    static inline uint32_t encode_zigzag32(int32_t n) noexcept;
    static inline uint64_t encode_zigzag64(int64_t n) noexcept;

    static inline int32_t decode_zigzag32(uint32_t n) noexcept;
    static inline int64_t decode_zigzag64(uint64_t n) noexcept;

    struct exception : std::exception { const char *what() const noexcept { return "pbf exception"; } };
    struct varint_too_long_exception : exception { const char *what() const noexcept { return "pbf varint too long exception"; } };
    struct unknown_field_type_exception : exception { const char *what() const noexcept { return "pbf unknown field type exception"; } };
    struct end_of_buffer_exception : exception { const char *what() const noexcept { return "pbf end of buffer exception"; } };

    inline pbf(const char *data, size_t length);
    inline pbf() = default;

    inline pbf(const pbf&) = default;
    inline pbf(pbf&&) = default;

    inline pbf& operator=(const pbf& other) = default;
    inline pbf& operator=(pbf&& other) = default;

    inline ~pbf() = default;

    inline uint32_t tag() const noexcept { return m_tag; }

    inline int wire_type() const noexcept;
    inline bool is_wire_type(int type) const noexcept;

    inline operator bool() const noexcept;

    inline bool next();
    inline bool next(uint32_t tag);

    inline int32_t get_int32() { return varint<int32_t>(); }
    inline int32_t get_sint32() { return svarint<int32_t>(); }
    inline uint32_t get_uint32() { return varint<uint32_t>(); }

    inline int64_t get_int64() { return varint<int64_t>(); }
    inline int64_t get_sint64() { return svarint<int64_t>(); }
    inline uint64_t get_uint64() { return varint<uint64_t>(); }

    inline uint32_t get_fixed32();
    inline int32_t get_sfixed32();
    inline uint64_t get_fixed64();
    inline int64_t get_sfixed64();

    inline float get_float();
    inline double get_double();

    inline bool get_bool();

    inline std::pair<const char*, uint32_t> get_data();
    inline std::string get_bytes();
    inline std::string get_string();
    inline pbf get_message();

    inline void skip();

    inline std::pair<const uint32_t*, const uint32_t*> packed_fixed32();
    inline std::pair<const uint64_t*, const uint64_t*> packed_fixed64();
    inline std::pair<const int32_t*, const int32_t*> packed_sfixed32();
    inline std::pair<const int64_t*, const int64_t*> packed_sfixed64();

    template <typename T>
    class const_varint_iterator : public std::iterator<std::forward_iterator_tag, T> {

    protected:

        const char* m_data;
        const char* m_end;

    public:

        const_varint_iterator() noexcept :
            m_data(nullptr),
            m_end(nullptr) {
        }

        const_varint_iterator(const char *data, const char* end) noexcept :
            m_data(data),
            m_end(end) {
        }

        const_varint_iterator(const const_varint_iterator&) = default;
        const_varint_iterator(const_varint_iterator&&) = default;

        const_varint_iterator& operator=(const const_varint_iterator&) = default;
        const_varint_iterator& operator=(const_varint_iterator&&) = default;

        ~const_varint_iterator() = default;

        T operator*() {
            const char* d = m_data; // will be thrown away
            return static_cast<T>(decode_varint(&d, m_end));
        }

        const_varint_iterator& operator++() {
            // Ignore the result, we call decode_varint() just for the
            // side-effect of updating data.
            decode_varint(&m_data, m_end);
            return *this;
        }

        const_varint_iterator operator++(int) noexcept {
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

        const_svarint_iterator() noexcept :
            const_varint_iterator<T>() {
        }

        const_svarint_iterator(const char *data, const char* end) noexcept :
            const_varint_iterator<T>(data, end) {
        }

        const_svarint_iterator(const const_svarint_iterator&) = default;
        const_svarint_iterator(const_svarint_iterator&&) = default;

        const_svarint_iterator& operator=(const const_svarint_iterator&) = default;
        const_svarint_iterator& operator=(const_svarint_iterator&&) = default;

        ~const_svarint_iterator() = default;

        T operator*() {
            const char* d = this->m_data; // will be thrown away
            return static_cast<T>(decode_zigzag64(decode_varint(&d, this->m_end)));
        }

        const_svarint_iterator& operator++() {
            // Ignore the result, we call decode_varint() just for the
            // side-effect of updating data.
            decode_varint(&this->m_data, this->m_end);
            return *this;
        }

        const_svarint_iterator operator++(int) noexcept {
            const const_svarint_iterator tmp(*this);
            ++(*this);
            return tmp;
        }

    }; // class const_svarint_iterator

    typedef const_varint_iterator< int32_t> const_int32_iterator;
    typedef const_varint_iterator<uint32_t> const_uint32_iterator;
    typedef const_svarint_iterator<int32_t> const_sint32_iterator;

    typedef const_varint_iterator< int64_t> const_int64_iterator;
    typedef const_varint_iterator<uint64_t> const_uint64_iterator;
    typedef const_svarint_iterator<int64_t> const_sint64_iterator;

    inline std::pair<pbf::const_int32_iterator,  pbf::const_int32_iterator>  packed_int32();
    inline std::pair<pbf::const_uint32_iterator, pbf::const_uint32_iterator> packed_uint32();
    inline std::pair<pbf::const_sint32_iterator, pbf::const_sint32_iterator> packed_sint32();

    inline std::pair<pbf::const_int64_iterator,  pbf::const_int64_iterator>  packed_int64();
    inline std::pair<pbf::const_uint64_iterator, pbf::const_uint64_iterator> packed_uint64();
    inline std::pair<pbf::const_sint64_iterator, pbf::const_sint64_iterator> packed_sint64();

}; // class pbf

pbf::pbf(const char *data, size_t length)
    : m_data(data),
      m_end(data + length),
      m_value(0),
      m_tag(0) {
}

int pbf::wire_type() const noexcept {
    return m_value & 0x7;
}

bool pbf::is_wire_type(int type) const noexcept {
    return wire_type() == type;
}

pbf::operator bool() const noexcept {
    return m_data < m_end;
}

bool pbf::next() {
    if (m_data < m_end) {
        m_value = get_uint32();
        m_tag = m_value >> 3;
        return true;
    }
    return false;
}

bool pbf::next(uint32_t requested_tag) {
    while (next()) {
        if (m_tag == requested_tag) {
            return true;
        } else {
            skip();
        }
    }
    return false;
}

template <typename T>
T pbf::varint() {
    return static_cast<T>(decode_varint(&m_data, m_end));
}

inline uint32_t pbf::encode_zigzag32(int32_t n) noexcept {
    return static_cast<uint32_t>(n << 1) ^ static_cast<uint32_t>(n >> 31);
}

inline uint64_t pbf::encode_zigzag64(int64_t n) noexcept {
    return static_cast<uint64_t>(n << 1) ^ static_cast<uint64_t>(n >> 63);
}

inline int32_t pbf::decode_zigzag32(uint32_t n) noexcept {
    return static_cast<int32_t>(n >> 1) ^ -static_cast<int32_t>((n & 1));
}

inline int64_t pbf::decode_zigzag64(uint64_t n) noexcept {
    return static_cast<int64_t>(n >> 1) ^ -static_cast<int64_t>((n & 1));
}

template <typename T>
T pbf::svarint() {
    return static_cast<T>(decode_zigzag64(decode_varint(&m_data, m_end)));
}

template <typename T>
T pbf::fixed() {
    skip_bytes(sizeof(T));
    T result;
    memcpy(&result, m_data - sizeof(T), sizeof(T));
    return result;
}

uint32_t pbf::get_fixed32() {
    assert(is_wire_type(5) && "not a 32-bit fixed");
    return fixed<uint32_t>();
}

int32_t pbf::get_sfixed32() {
    assert(is_wire_type(5) && "not a 32-bit fixed");
    return fixed<int32_t>();
}

uint64_t pbf::get_fixed64() {
    assert(is_wire_type(1) && "not a 64-bit fixed");
    return fixed<uint64_t>();
}

int64_t pbf::get_sfixed64() {
    assert(is_wire_type(1) && "not a 64-bit fixed");
    return fixed<int64_t>();
}

float pbf::get_float() {
    assert(is_wire_type(5) && "not a 32-bit fixed");
    return fixed<float>();
}

double pbf::get_double() {
    assert(is_wire_type(1) && "not a 64-bit fixed");
    return fixed<double>();
}

bool pbf::get_bool() {
    assert(is_wire_type(0) && "not a varint");
    assert((*m_data & 0x80) == 0 && "not a 1 byte varint");
    skip_bytes(1);
    return *reinterpret_cast<const bool *>(m_data - 1);
}

std::pair<const char*, uint32_t> pbf::get_data() {
    assert(is_wire_type(2) && "not of type string, bytes or message");
    auto len = get_len_and_skip();
    return std::make_pair(m_data-len, len);
}

std::string pbf::get_bytes() {
    auto d = get_data();
    return std::string(d.first, d.second);
}

std::string pbf::get_string() {
    return get_bytes();
}

pbf pbf::get_message() {
    auto d = get_data();
    return pbf(d.first, d.second);
}

void pbf::skip_bytes(uint32_t len) {
    if (m_data + len > m_end) {
        throw end_of_buffer_exception();
    }
    m_data += len;
}

void pbf::skip() {
    switch (m_value & 0x7) {
        case 0: // varint
            get_uint32();
            break;
        case 1: // 64 bit
            skip_bytes(8);
            break;
        case 2: // string/bytes/message
            skip_bytes(get_uint32());
            break;
        case 5: // 32 bit
            skip_bytes(4);
            break;
        default:
            throw unknown_field_type_exception();
    }
}

uint32_t pbf::get_len_and_skip() {
    uint32_t len = get_uint32();
    skip_bytes(len);
    return len;
}

template <typename T>
std::pair<const T*, const T*> pbf::packed_fixed() {
    auto len = get_len_and_skip();
    assert(len % sizeof(T) == 0);
    return std::make_pair(reinterpret_cast<const T*>(m_data-len), reinterpret_cast<const T*>(m_data));
}

std::pair<const uint32_t*, const uint32_t*> pbf::packed_fixed32() {
    return packed_fixed<uint32_t>();
}

std::pair<const uint64_t*, const uint64_t*> pbf::packed_fixed64() {
    return packed_fixed<uint64_t>();
}

std::pair<const int32_t*, const int32_t*> pbf::packed_sfixed32() {
    return packed_fixed<int32_t>();
}

std::pair<const int64_t*, const int64_t*> pbf::packed_sfixed64() {
    return packed_fixed<int64_t>();
}

std::pair<pbf::const_int32_iterator, pbf::const_int32_iterator> pbf::packed_int32() {
    auto len = get_len_and_skip();
    return std::make_pair(pbf::const_int32_iterator(m_data-len, m_data),
                          pbf::const_int32_iterator(m_data, m_data));
}

std::pair<pbf::const_uint32_iterator, pbf::const_uint32_iterator> pbf::packed_uint32() {
    auto len = get_len_and_skip();
    return std::make_pair(pbf::const_uint32_iterator(m_data-len, m_data),
                          pbf::const_uint32_iterator(m_data, m_data));
}

std::pair<pbf::const_sint32_iterator, pbf::const_sint32_iterator> pbf::packed_sint32() {
    auto len = get_len_and_skip();
    return std::make_pair(pbf::const_sint32_iterator(m_data-len, m_data),
                          pbf::const_sint32_iterator(m_data, m_data));
}

std::pair<pbf::const_int64_iterator, pbf::const_int64_iterator> pbf::packed_int64() {
    auto len = get_len_and_skip();
    return std::make_pair(pbf::const_int64_iterator(m_data-len, m_data),
                          pbf::const_int64_iterator(m_data, m_data));
}

std::pair<pbf::const_uint64_iterator, pbf::const_uint64_iterator> pbf::packed_uint64() {
    auto len = get_len_and_skip();
    return std::make_pair(pbf::const_uint64_iterator(m_data-len, m_data),
                          pbf::const_uint64_iterator(m_data, m_data));
}

std::pair<pbf::const_sint64_iterator, pbf::const_sint64_iterator> pbf::packed_sint64() {
    auto len = get_len_and_skip();
    return std::make_pair(pbf::const_sint64_iterator(m_data-len, m_data),
                          pbf::const_sint64_iterator(m_data, m_data));
}

}} // end namespace mapbox::util

#endif // MAPBOX_UTIL_PBF_HPP
