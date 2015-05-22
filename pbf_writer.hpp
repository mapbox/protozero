#ifndef MAPBOX_UTIL_PBF_WRITER_HPP
#define MAPBOX_UTIL_PBF_WRITER_HPP

/*****************************************************************************

Minimalistic fast C++ encoder for a subset of the protocol buffer format.

This is header-only, meaning there is nothing to build. Just include this file
in your C++ application.

This file is from https://github.com/mapbox/pbf.hpp where you can find more
documentation.

*****************************************************************************/

#ifndef assert
# include <cassert>
#endif

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iterator>
#include <string>

#include "pbf_common.hpp"

namespace mapbox { namespace util {

/**
 * The pbf_writer is used to write PBF formatted messages into a buffer.
 */
class pbf_writer {

    std::string& m_data;

    template <typename T>
    static inline int write_varint(T data, uint64_t value) {
        int n=1;

        while (value >= 0x80) {
            *data++ = char((value & 0x7f) | 0x80);
            value >>= 7;
            ++n;
        }
        *data++ = char(value);

        return n;
    }

    inline int add_varint(uint64_t value) {
        return write_varint(std::back_inserter(m_data), value);
    }

    inline void add_tagged_varint(pbf_tag_type tag, uint64_t value) {
        add_field(tag, pbf_wire_type::varint);
        add_varint(value);
    }

    inline void add_field(pbf_tag_type tag, pbf_wire_type type) {
        uint32_t b = (tag << 3) | uint32_t(type);
        add_varint(b);
    }

    template <typename T>
    inline void add_data(T value) {
        m_data.append(reinterpret_cast<const char*>(&value), sizeof(T));
    }

    template <typename T>
    inline void add_packed_fixed(pbf_tag_type tag, T* begin, T* end) {
        if (end == begin) {
            return;
        }
        add_field(tag, pbf_wire_type::length_delimited);
        add_varint(sizeof(T) * uint32_t(std::distance(begin, end)));

        for (; begin != end; ++begin) {
            m_data.append(reinterpret_cast<const char*>(begin), sizeof(T));
        }
    }

    template <typename T>
    inline void add_packed_varint(pbf_tag_type tag, T* begin, T* end) {
        if (end == begin) {
            return;
        }

        std::string data;
        pbf_writer w(data);
        for (; begin != end; ++begin) {
            w.add_varint(uint64_t(*begin));
        }

        add_field(tag, pbf_wire_type::length_delimited);
        add_varint(data.size());
        m_data.append(data);
    }

    template <typename T>
    inline void add_packed_svarint(pbf_tag_type tag, T* begin, T* end) {
        if (end == begin) {
            return;
        }

        std::string data;
        pbf_writer w(data);
        for (; begin != end; ++begin) {
            w.add_varint(encode_zigzag64(int64_t(*begin)));
        }

        add_field(tag, pbf_wire_type::length_delimited);
        add_varint(data.size());
        m_data.append(data);
    }

    static const int reserve_bytes = 10;

    inline void reserve_space() {
        m_data.append(size_t(reserve_bytes), '\0');
    }

public:

    /**
     * ZigZag encodes a 32 bit integer.
     *
     * This is a helper function used inside the pbf_writer class, but could be
     * useful in other contexts.
     */
    static inline uint32_t encode_zigzag32(int32_t n) noexcept;

    /**
     * ZigZag encodes a 64 bit integer.
     *
     * This is a helper function used inside the pbf_writer class, but could be
     * useful in other contexts.
     */
    static inline uint64_t encode_zigzag64(int64_t n) noexcept;

    /**
     * Create a writer using the given string as a data store. The pbf_writer
     * stores a reference to that string and adds all data to it.
     */
    inline pbf_writer(std::string& data) :
        m_data(data) {
    }

    pbf_writer(const pbf_writer&) = delete;
    pbf_writer& operator=(const pbf_writer&) = delete;

    inline pbf_writer(pbf_writer&&) = default;
    inline pbf_writer& operator=(pbf_writer&&) = default;

    inline ~pbf_writer() = default;

    ///@{
    /**
     * @name Scalar field writer functions
     */

    inline void add_bool(pbf_tag_type tag, bool value) {
        add_field(tag, pbf_wire_type::varint);
        m_data.append(1, char(value));
    }

    inline void add_enum(pbf_tag_type tag, int32_t value) {
        add_tagged_varint(tag, uint64_t(value));
    }

    inline void add_int32(pbf_tag_type tag, int32_t value) {
        add_tagged_varint(tag, uint64_t(value));
    }

    inline void add_sint32(pbf_tag_type tag, int32_t value) {
        add_tagged_varint(tag, encode_zigzag32(value));
    }

    inline void add_uint32(pbf_tag_type tag, uint32_t value) {
        add_tagged_varint(tag, value);
    }

    inline void add_int64(pbf_tag_type tag, int64_t value) {
        add_tagged_varint(tag, uint64_t(value));
    }

    inline void add_sint64(pbf_tag_type tag, int64_t value) {
        add_tagged_varint(tag, encode_zigzag64(value));
    }

    inline void add_uint64(pbf_tag_type tag, uint64_t value) {
        add_tagged_varint(tag, value);
    }

    inline void add_fixed32(pbf_tag_type tag, uint32_t value) {
        add_field(tag, pbf_wire_type::fixed32);
        add_data<uint32_t>(value);
    }

    inline void add_sfixed32(pbf_tag_type tag, int32_t value) {
        add_field(tag, pbf_wire_type::fixed32);
        add_data<int32_t>(value);
    }

    inline void add_fixed64(pbf_tag_type tag, uint64_t value) {
        add_field(tag, pbf_wire_type::fixed64);
        add_data<uint64_t>(value);
    }

    inline void add_sfixed64(pbf_tag_type tag, int64_t value) {
        add_field(tag, pbf_wire_type::fixed64);
        add_data<int64_t>(value);
    }

    inline void add_float(pbf_tag_type tag, float value) {
        add_field(tag, pbf_wire_type::fixed32);
        add_data<float>(value);
    }

    inline void add_double(pbf_tag_type tag, double value) {
        add_field(tag, pbf_wire_type::fixed64);
        add_data<double>(value);
    }

    inline void add_bytes(pbf_tag_type tag, const std::string& value) {
        add_field(tag, pbf_wire_type::length_delimited);
        add_varint(value.size());
        m_data.append(value);
    }

    inline void add_bytes(pbf_tag_type tag, const char* value, size_t size) {
        add_field(tag, pbf_wire_type::length_delimited);
        add_varint(size);
        m_data.append(value, size);
    }

    inline void add_string(pbf_tag_type tag, const std::string& value) {
        add_bytes(tag, value);
    }

    inline void add_string(pbf_tag_type tag, const char* value, size_t size) {
        add_bytes(tag, value, size);
    }

    inline void add_string(pbf_tag_type tag, const char* value) {
        add_field(tag, pbf_wire_type::length_delimited);
        add_varint(std::strlen(value));
        m_data.append(value);
    }

    inline void add_message(pbf_tag_type tag, const std::string& value) {
        add_bytes(tag, value);
    }

    ///@}

    ///@{
    /**
     * @name Repeated packed field writer functions
     */

    inline void add_packed_fixed32(pbf_tag_type tag, uint32_t* begin, uint32_t* end) {
        add_packed_fixed<uint32_t>(tag, begin, end);
    }

    inline void add_packed_fixed64(pbf_tag_type tag, uint64_t* begin, uint64_t* end) {
        add_packed_fixed<uint64_t>(tag, begin, end);
    }

    inline void add_packed_sfixed32(pbf_tag_type tag, int32_t* begin, int32_t* end) {
        add_packed_fixed<int32_t>(tag, begin, end);
    }

    inline void add_packed_sfixed64(pbf_tag_type tag, int64_t* begin, int64_t* end) {
        add_packed_fixed<int64_t>(tag, begin, end);
    }

    inline void add_packed_int32(pbf_tag_type tag, int32_t* begin, int32_t* end) {
        add_packed_varint<int32_t>(tag, begin, end);
    }

    inline void add_packed_uint32(pbf_tag_type tag, uint32_t* begin, uint32_t* end) {
        add_packed_varint<uint32_t>(tag, begin, end);
    }

    inline void add_packed_sint32(pbf_tag_type tag, int32_t* begin, int32_t* end) {
        add_packed_svarint<int32_t>(tag, begin, end);
    }

    inline void add_packed_int64(pbf_tag_type tag, int64_t* begin, int64_t* end) {
        add_packed_varint<int64_t>(tag, begin, end);
    }

    inline void add_packed_uint64(pbf_tag_type tag, uint64_t* begin, uint64_t* end) {
        add_packed_varint<uint64_t>(tag, begin, end);
    }

    inline void add_packed_sint64(pbf_tag_type tag, int64_t* begin, int64_t* end) {
        add_packed_svarint<int64_t>(tag, begin, end);
    }

    ///@}

    inline size_t open_sub(pbf_tag_type tag) {
        add_field(tag, pbf_wire_type::length_delimited);
        reserve_space();
        return m_data.size();
    }

    inline void close_sub(size_t pos) {
        auto length = uint32_t(m_data.size() - pos);

        assert(m_data.size() >= pos - reserve_bytes);
        int n = write_varint(&m_data[pos - reserve_bytes], length);

        m_data.erase(m_data.begin() + long(pos) - reserve_bytes + n, m_data.begin() + long(pos));
    }

    void append_sub(const std::string& value) {
        m_data.append(value);
    }

    void append_sub(const char* value, size_t size) {
        m_data.append(value, size);
    }

}; // class pbf_writer


class pbf_subwriter {

    pbf_writer& m_writer;
    size_t m_pos = 0;

public:

    inline pbf_subwriter(pbf_writer& writer, pbf_tag_type tag) :
        m_writer(writer),
        m_pos(writer.open_sub(tag)) {
    }

    pbf_subwriter(const pbf_subwriter&) = delete;
    pbf_subwriter& operator=(const pbf_subwriter&) = delete;

    inline pbf_subwriter(pbf_subwriter&&) = default;
    inline pbf_subwriter& operator=(pbf_subwriter&&) = default;

    inline ~pbf_subwriter() {
        m_writer.close_sub(m_pos);
    }

    inline void append(const std::string& value) {
        m_writer.append_sub(value);
    }

    inline void append(const char* value, size_t size) {
        m_writer.append_sub(value, size);
    }

}; // class pbf_subwriter

inline uint32_t pbf_writer::encode_zigzag32(int32_t n) noexcept {
    return static_cast<uint32_t>(n << 1) ^ static_cast<uint32_t>(n >> 31);
}

inline uint64_t pbf_writer::encode_zigzag64(int64_t n) noexcept {
    return static_cast<uint64_t>(n << 1) ^ static_cast<uint64_t>(n >> 63);
}

}} // end namespace mapbox::util

#endif // MAPBOX_UTIL_PBF_WRITER_HPP
