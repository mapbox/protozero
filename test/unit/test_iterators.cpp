
#include <test.hpp>

TEST_CASE("default constructed varint_iterators are equal") {
    const protozero::const_varint_iterator<uint32_t> a{};
    const protozero::const_varint_iterator<uint32_t> b{};

    const protozero::iterator_range<protozero::const_varint_iterator<uint32_t>> r{};

    REQUIRE(a == a);
    REQUIRE(a == b);
    REQUIRE(a == r.begin());
    REQUIRE(a == r.end());
    REQUIRE(r.empty());
    REQUIRE(r.size() == 0); // NOLINT(readability-container-size-empty)
    REQUIRE(r.begin() == r.end());
}

TEST_CASE("get_packed varint reads a well-formed region") {
    // field 1, wire type 2 (length-delimited); length 2; varints 5 and 1.
    const std::string buffer{"\x0a\x02\x05\x01", 4};
    protozero::pbf_reader item{buffer};

    REQUIRE(item.next());
    const auto range = item.get_packed_uint64();
    auto it = range.begin();
    REQUIRE(*it++ == 5);
    REQUIRE(*it++ == 1);
    REQUIRE(it == range.end());
}

TEST_CASE("get_packed varint rejects a truncated trailing varint") {
    // field 1, wire type 2; length 1; single byte 0x80 with the continuation
    // bit set and no terminating byte: the region does not end on a varint
    // boundary, so this must be rejected when the range is created.
    const std::string buffer{"\x0a\x01\x80", 3};
    protozero::pbf_reader item{buffer};

    REQUIRE(item.next());
    REQUIRE_THROWS_AS(item.get_packed_uint64(), protozero::end_of_buffer_exception);
}

