
#include <buffer.hpp>

#include <protozero/buffer_string.hpp>
#include <protozero/buffer_vector.hpp>
#include <protozero/fixed_size_buffer_adaptor.hpp>

#include <algorithm>
#include <array>
#include <stdexcept>
#include <type_traits>

TEMPLATE_TEST_CASE("Use various buffer types", "", buffer_test_string, buffer_test_vector, buffer_test_array, buffer_test_external) {
    // this is needed for std::back_inserter() to work
    static_assert(std::is_same<typename TestType::type::value_type, char>::value, "Buffer types used must have value_type char");

    TestType tt;
    auto* buffer = &tt.buffer();

    using protozero::buffer_size;
    using protozero::buffer_append;
    using protozero::buffer_append_zeros;
    using protozero::buffer_resize;
    using protozero::buffer_reserve_additional;
    using protozero::buffer_erase_range;
    using protozero::buffer_at_pos;

    REQUIRE(buffer_size(buffer) == 0);

    buffer_append(buffer, "abc def ghi", 11);
    REQUIRE(buffer_size(buffer) == 11);

    buffer_append_zeros(buffer, 3);
    REQUIRE(buffer_size(buffer) == 14);

    buffer_resize(buffer, 11);
    REQUIRE(buffer_size(buffer) == 11);

    buffer_append(buffer, " jkl", 4);
    REQUIRE(buffer_size(buffer) == 15);

    buffer_erase_range(buffer, 4, 8);
    REQUIRE(buffer_size(buffer) == 11);
    REQUIRE(std::equal(buffer_at_pos(buffer, 0),
                       buffer_at_pos(buffer, buffer_size(buffer)),
                       "abc ghi jkl"));

    buffer->push_back(' ');
    buffer->push_back('x');
    buffer->push_back('y');
    REQUIRE(buffer_size(buffer) == 14);
    REQUIRE(std::equal(buffer_at_pos(buffer, 0),
                       buffer_at_pos(buffer, buffer_size(buffer)),
                       "abc ghi jkl xy"));
}

TEST_CASE("fixed_size_buffer_adaptor has limited size") {
    std::array<char, 5> data = {{0}};
    protozero::fixed_size_buffer_adaptor fsba{&*data.begin(), data.size()};
    REQUIRE_THROWS_AS(protozero::buffer_append(&fsba, "0123456789", 10), std::length_error);
}
