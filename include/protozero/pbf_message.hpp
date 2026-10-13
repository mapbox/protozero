#ifndef PROTOZERO_PBF_MESSAGE_HPP
#define PROTOZERO_PBF_MESSAGE_HPP

/*****************************************************************************

protozero - Minimalistic protocol buffer decoder and encoder in C++.

This file is from https://github.com/mapbox/protozero where you can find more
documentation.

*****************************************************************************/

/**
 * @file pbf_message.hpp
 *
 * @brief Contains the pbf_message template class.
 */

#include "pbf_reader.hpp"
#include "types.hpp"

#include <type_traits>

namespace protozero {

/**
 * This class represents a protobuf message. Either a top-level message or
 * a nested sub-message. Top-level messages can be created from any buffer
 * with a pointer and length:
 *
 * @code
 *    enum class Message : protozero::pbf_tag_type {
 *       ...
 *    };
 *
 *    std::string buffer;
 *    // fill buffer...
 *    pbf_message<Message> message{buffer.data(), buffer.size()};
 * @endcode
 *
 * Sub-messages are created using get_message():
 *
 * @code
 *    enum class SubMessage : protozero::pbf_tag_type {
 *       ...
 *    };
 *
 *    pbf_message<Message> message{...};
 *    message.next();
 *    pbf_message<SubMessage> submessage = message.get_message();
 * @endcode
 *
 * All methods of the pbf_message class except get_bytes() and get_string()
 * provide the strong exception guarantee, ie they either succeed or do not
 * change the pbf_message object they are called on. Use the get_data() method
 * instead of get_bytes() or get_string(), if you need this guarantee.
 *
 * This template class is based on the pbf_reader class and has all the same
 * methods. The difference is that whereever the pbf_reader class takes an
 * integer tag, this template class takes a tag of the template type T.
 *
 * Read the tutorial to understand how this class is used.
 */
template <typename T>
class pbf_message : public pbf_reader {

    static_assert(std::is_same<pbf_tag_type, std::underlying_type_t<T>>::value,
                  "T must be enum with underlying type protozero::pbf_tag_type");

public:

    /// The type of messages this class will read.
    using enum_type = T;

    /**
     * Construct a pbf_message. All arguments are forwarded to the pbf_reader
     * parent class.
     */
    template <typename... Args>
    pbf_message(Args&&... args) noexcept : // NOLINT(google-explicit-constructor, hicpp-explicit-conversions)
        pbf_reader{std::forward<Args>(args)...} {
    }

    /**
     * Set next field in the message as the current field. This is usually
     * called in a while loop:
     *
     * @code
     *    pbf_message<...> message(...);
     *    while (message.next()) {
     *        // handle field
     *    }
     * @endcode
     *
     * @returns `true` if there is a next field, `false` if not.
     * @pre There must be no current field.
     * @post If it returns `true` there is a current field now.
     */
    bool next() {
        return pbf_reader::next();
    }

    /**
     * Set next field with given tag in the message as the current field.
     * Fields with other tags are skipped.
     *
     * Note that this will not check the wire type. The input data can
     * contain a field with this tag but an unexpected wire type, so check
     * the wire type before calling an accessor function. This is useful
     * for repeated scalar fields which can be encoded packed or not packed:
     *
     * @code
     *    pbf_message<Example1> message{...};
     *    while (message.next(Example1::repeated_uint32_x)) {
     *        if (message.wire_type() == pbf_wire_type::length_delimited) {
     *            auto range = message.get_packed_uint32();
     *            // handle packed values
     *        } else if (message.wire_type() == pbf_wire_type::varint) {
     *            auto value = message.get_uint32();
     *            // handle single value
     *        } else {
     *            message.skip();
     *        }
     *    }
     * @endcode
     *
     * Use the two-argument version next(tag, type) if you only accept one
     * wire type.
     *
     * @returns `true` if there is a next field with this tag.
     * @pre There must be no current field.
     * @post If it returns `true` there is a current field now with the given tag.
     */
    bool next(T next_tag) {
        return pbf_reader::next(pbf_tag_type(next_tag));
    }

    /**
     * Set next field with given tag and wire type in the message as the
     * current field. Fields with other tags or other wire types are skipped.
     * This is usually called in a while loop for repeated fields:
     *
     * @code
     *    pbf_message<Example1> message{...};
     *    while (message.next(Example1::repeated_fixed64_r, pbf_wire_type::varint)) {
     *        // handle field
     *    }
     * @endcode
     *
     * or you can call it just once to get the one field with this tag:
     *
     * @code
     *    pbf_message<Example1> message{...};
     *    if (message.next(Example1::required_uint32_x, pbf_wire_type::varint)) {
     *        // handle field
     *    }
     * @endcode
     *
     * Note that repeated scalar fields can be encoded packed (wire type
     * `length_delimited`) or not packed and a reader must accept both. Use
     * tag_and_type() if you need to handle both encodings.
     *
     * @returns `true` if there is a next field with this tag.
     * @pre There must be no current field.
     * @post If it returns `true` there is a current field now with the given tag.
     */
    bool next(T next_tag, pbf_wire_type type) {
        return pbf_reader::next(pbf_tag_type(next_tag), type);
    }

    /**
     * The tag of the current field. The tag is the enum value for the field
     * number from the description in the .proto file.
     *
     * Call next() before calling this function to set the current field.
     *
     * If you want to decide which accessor function to call based on the
     * tag, use tag_and_type() instead. It also checks the wire type, so
     * fields with the expected tag but an unexpected wire type in the input
     * data will not be misinterpreted.
     *
     * @returns tag of the current field.
     * @pre There must be a current field (ie. next() must have returned `true`).
     */
    T tag() const noexcept {
        return T(pbf_reader::tag());
    }

}; // class pbf_message

} // end namespace protozero

#endif // PROTOZERO_PBF_MESSAGE_HPP
