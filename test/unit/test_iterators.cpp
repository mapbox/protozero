
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


TEST_CASE("fixed_iterator supports negative offsets") {
    // field 1, wire type 2; length 12; fixed32 values 1, 2 and 3.
    const std::string buffer{"\x0a\x0c\x01\x00\x00\x00\x02\x00\x00\x00\x03\x00\x00\x00", 14};
    protozero::pbf_reader item{buffer};

    REQUIRE(item.next());
    const auto range = item.get_packed_fixed32();
    const auto end = range.end();

    REQUIRE(*std::prev(end) == 3);
    REQUIRE(end[-1] == 3);
    REQUIRE(end[-3] == 1);
    REQUIRE(*(end + -2) == 2);
    REQUIRE(*(-2 + end) == 2);
    REQUIRE(*(end - 2) == 2);
    REQUIRE(*(range.begin() - -1) == 2);

    auto it = end;
    it += -1;
    REQUIRE(*it == 3);
    it -= 1;
    REQUIRE(*it == 2);
    it -= -1;
    REQUIRE(*it == 3);
    REQUIRE(it - end == -1);
    REQUIRE(range.begin() - end == -3);
}
