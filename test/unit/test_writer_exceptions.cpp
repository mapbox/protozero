#include <test.hpp>

#include <protozero/buffer_fixed.hpp>

#include <array>
#include <stdexcept>
#include <string>

namespace {

    std::string to_string(const protozero::fixed_size_buffer_adaptor& buffer) {
        return std::string{buffer.data(), buffer.size()};
    }

    using fixed_writer = protozero::basic_pbf_writer<protozero::fixed_size_buffer_adaptor>;

} // anonymous namespace

TEST_CASE("full fixed size buffer while opening submessage leaves parent unchanged") {
    std::array<char, 4> data = {{0}};
    protozero::fixed_size_buffer_adaptor adaptor{data.data(), data.size()};
    fixed_writer pw{adaptor};
    pw.add_uint32(1, 17);

    // The field key fits, but the space reserved for the length does not.
    REQUIRE_THROWS_AS(fixed_writer(pw, 2), std::length_error);
    REQUIRE(adaptor.size() == 2);

    // The parent can still be used.
    pw.add_uint32(3, 18);
    REQUIRE(to_string(adaptor) == std::string{"\x08\x11\x18\x12", 4});
}

TEST_CASE("full fixed size buffer while opening submessage with known size leaves parent unchanged") {
    std::array<char, 4> data = {{0}};
    protozero::fixed_size_buffer_adaptor adaptor{data.data(), data.size()};
    fixed_writer pw{adaptor};
    pw.add_uint32(1, 17);

    // The field key and the first byte of the two-byte length fit, but the
    // second byte of the length does not.
    REQUIRE_THROWS_AS(fixed_writer(pw, 2, 200), std::length_error);
    REQUIRE(adaptor.size() == 2);

    pw.add_uint32(3, 18);
    REQUIRE(to_string(adaptor) == std::string{"\x08\x11\x18\x12", 4});
}

TEST_CASE("submessage can be rolled back after an exception while writing it") {
    std::array<char, 10> data = {{0}};
    protozero::fixed_size_buffer_adaptor adaptor{data.data(), data.size()};
    fixed_writer pw{adaptor};
    pw.add_uint32(1, 17);

    {
        fixed_writer sub{pw, 2};
        REQUIRE_THROWS_AS(sub.add_string(1, "too long for the buffer"), std::length_error);
        sub.rollback();
    }
    REQUIRE(adaptor.size() == 2);

    pw.add_uint32(3, 18);
    REQUIRE(to_string(adaptor) == std::string{"\x08\x11\x18\x12", 4});
}
