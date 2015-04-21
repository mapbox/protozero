#ifndef __PBF_HPP__
#define __PBF_HPP__

/*
 * Some parts are from upb - a minimalist implementation of protocol buffers.
 *
 * Copyright (c) 2008-2011 Google Inc.  See LICENSE for details.
 * Author: Josh Haberman <jhaberman@gmail.com>
 */

#include <stdint.h>
#include <stdexcept>
#include <string>
#include <cstring>
#include <cassert>

#undef LIKELY
#undef UNLIKELY

#if defined(__GNUC__) && __GNUC__ >= 4
#define LIKELY(x)   (__builtin_expect((x), 1))
#define UNLIKELY(x) (__builtin_expect((x), 0))
#else
#define LIKELY(x)   (x)
#define UNLIKELY(x) (x)
#endif

namespace protobuf {

#define FORCEINLINE inline __attribute__((always_inline))
#define NOINLINE __attribute__((noinline))
#define PBF_INLINE FORCEINLINE

class message {
    typedef const char * value_type;
    value_type data_;
    value_type end_;
public:
    uint64_t value;
    uint32_t tag;

    PBF_INLINE message(value_type data, std::size_t length);

    PBF_INLINE bool next();
    PBF_INLINE uint64_t varint();
    PBF_INLINE int64_t svarint();
    PBF_INLINE std::string string();
    PBF_INLINE float float32();
    PBF_INLINE double float64();
    PBF_INLINE int64_t int64();
    PBF_INLINE bool boolean();
    PBF_INLINE void skip();
    PBF_INLINE void skipValue(uint64_t val);
    PBF_INLINE void skipBytes(uint64_t bytes);
    PBF_INLINE value_type getData();
};

message::message(value_type data, std::size_t length)
    : data_(data),
      end_(data + length)
{
}

bool message::next()
{
    if (data_ < end_) {
        value = varint();
        tag = static_cast<uint32_t>(value >> 3);
        return true;
    }
    return false;
}


uint64_t message::varint()
{
    int8_t byte = static_cast<int8_t>(0x80);
    uint64_t result = 0;
    int bitpos;
    for (bitpos = 0; bitpos < 70 && (byte & 0x80); bitpos += 7) {
        if (data_ >= end_) {
            throw std::runtime_error("unterminated varint, unexpected end of buffer");
        }
        result |= ((uint64_t)(byte = *data_) & 0x7F) << bitpos;
        data_++;
    }
    if (bitpos == 70 && (byte & 0x80)) {
        throw std::runtime_error("unterminated varint (too long)");
    }

    return result;
}

int64_t message::svarint()
{
    uint64_t n = varint();
    return (n >> 1) ^ -static_cast<int64_t>((n & 1));
}

std::string message::string()
{
    uint64_t len = varint();
    value_type string = static_cast<value_type>(data_);
    skipBytes(len);
    return std::string(string, len);
}

float message::float32()
{
    skipBytes(4);
    float result;
    std::memcpy(&result, data_ - 4, 4);
    return result;
}
double message::float64()
{
    skipBytes(8);
    double result;
    std::memcpy(&result, data_ - 8, 8);
    return result;
}

int64_t message::int64()
{
    return (int64_t)varint();
}

bool message::boolean()
{
    skipBytes(1);
    return *(bool *)(data_ - 1);
}

void message::skip()
{
    skipValue(value);
}

void message::skipValue(uint64_t val)
{
    switch (val & 0x7) {
        case 0: // varint
            varint();
            break;
        case 1: // 64 bit
            skipBytes(8);
            break;
        case 2: // string/message
            skipBytes(varint());
            break;
        case 5: // 32 bit
            skipBytes(4);
            break;
        default:
            char msg[80];
            snprintf(msg, 80, "cannot skip unknown type %lld", val & 0x7);
            throw std::runtime_error(msg);
    }
}

void message::skipBytes(uint64_t bytes)
{
    data_ += bytes;
    if (data_ > end_) {
        throw std::runtime_error("unexpected end of buffer");
    }
}

message::value_type message::getData()
{
  return data_;
}

}

#endif // __PBF_HPP__