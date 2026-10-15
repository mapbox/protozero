
#include <test.hpp>

// The pbf_reader promises the strong exception guarantee: If a method
// throws, the reader is unchanged.

namespace {

    void require_unchanged(const protozero::pbf_reader& before, const protozero::pbf_reader& after) {
        REQUIRE(after.length() == before.length());
        REQUIRE(after.data().data() == before.data().data());
        REQUIRE(after.tag() == before.tag());
        REQUIRE(after.wire_type() == before.wire_type());
    }

} // anonymous namespace

TEST_CASE("next() on invalid tag 0 does not change reader") {
    // field 1 (varint) = 1, then tag 0 (fixed64)
    const std::string buffer{"\x08\x01\x01\x00", 4};
    protozero::pbf_reader item{buffer};
    REQUIRE(item.next());
    REQUIRE(item.get_int32() == 1);

    const auto before = item;
    REQUIRE_THROWS_AS(item.next(), protozero::invalid_tag_exception);
    require_unchanged(before, item);
}

TEST_CASE("next() on reserved tag does not change reader") {
    std::string buffer;
    protozero::add_varint_to_buffer(&buffer, 19000U << 3U | 1U);
    protozero::pbf_reader item{buffer};

    const auto before = item;
    REQUIRE_THROWS_AS(item.next(), protozero::invalid_tag_exception);
    require_unchanged(before, item);
}

TEST_CASE("next() on unknown wire type does not change reader") {
    // field 1 (varint) = 1, then field 1 with wire type 7
    const std::string buffer{"\x08\x01\x0f", 3};
    protozero::pbf_reader item{buffer};
    REQUIRE(item.next());
    REQUIRE(item.get_int32() == 1);

    const auto before = item;
    REQUIRE_THROWS_AS(item.next(), protozero::unknown_pbf_wire_type_exception);
    require_unchanged(before, item);
}

TEST_CASE("next(tag) does not skip fields if a later field is invalid") {
    // field 1 (varint) = 1, field 2 (length-delimited) with length 5, but
    // only one byte of data
    const std::string buffer{"\x08\x01\x12\x05x", 5};

    SECTION("next(tag)") {
        protozero::pbf_reader item{buffer};
        const auto before = item;
        REQUIRE_THROWS_AS(item.next(3), protozero::end_of_buffer_exception);
        require_unchanged(before, item);
    }

    SECTION("next(tag, type)") {
        protozero::pbf_reader item{buffer};
        const auto before = item;
        REQUIRE_THROWS_AS(item.next(3, protozero::pbf_wire_type::varint), protozero::end_of_buffer_exception);
        require_unchanged(before, item);
    }
}

TEST_CASE("length-delimited field with truncated data does not change reader") {
    // field 1 (length-delimited) with length 5, but only one byte of data
    const std::string buffer{"\x0a\x05x", 3};
    protozero::pbf_reader item{buffer};
    REQUIRE(item.next());
    const auto before = item;

    SECTION("skip") {
        REQUIRE_THROWS_AS(item.skip(), protozero::end_of_buffer_exception);
    }

    SECTION("get_view") {
        REQUIRE_THROWS_AS(item.get_view(), protozero::end_of_buffer_exception);
    }

#ifndef PROTOZERO_STRICT_API
    SECTION("get_data") {
        REQUIRE_THROWS_AS(item.get_data(), protozero::end_of_buffer_exception);
    }
#endif

    SECTION("get_string") {
        REQUIRE_THROWS_AS(item.get_string(), protozero::end_of_buffer_exception);
    }

    SECTION("get_message") {
        REQUIRE_THROWS_AS(item.get_message(), protozero::end_of_buffer_exception);
    }

    SECTION("get_packed_int32") {
        REQUIRE_THROWS_AS(item.get_packed_int32(), protozero::end_of_buffer_exception);
    }

    SECTION("get_packed_fixed32") {
        REQUIRE_THROWS_AS(item.get_packed_fixed32(), protozero::end_of_buffer_exception);
    }

    require_unchanged(before, item);
}

TEST_CASE("packed varint field with truncated last varint does not change reader") {
    // field 1 (length-delimited) with length 2, last byte has the
    // continuation bit set
    const std::string buffer{"\x0a\x02\x01\x80", 4};
    protozero::pbf_reader item{buffer};
    REQUIRE(item.next());
    const auto before = item;

    SECTION("get_packed_int32") {
        REQUIRE_THROWS_AS(item.get_packed_int32(), protozero::end_of_buffer_exception);
    }

    SECTION("get_packed_sint64") {
        REQUIRE_THROWS_AS(item.get_packed_sint64(), protozero::end_of_buffer_exception);
    }

    require_unchanged(before, item);
}

TEST_CASE("packed fixed field with invalid length does not change reader") {
    // field 1 (length-delimited) with length 3, which is not a multiple of 4
    const std::string buffer{"\x0a\x03\x01\x02\x03", 5};
    protozero::pbf_reader item{buffer};
    REQUIRE(item.next());
    const auto before = item;

    SECTION("get_packed_fixed32") {
        REQUIRE_THROWS_AS(item.get_packed_fixed32(), protozero::invalid_length_exception);
    }

    SECTION("get_packed_double") {
        REQUIRE_THROWS_AS(item.get_packed_double(), protozero::invalid_length_exception);
    }

    require_unchanged(before, item);
}

