
#include <test.hpp>

TEST_CASE("varint") {

    std::string buffer;
    protozero::pbf_writer pw(buffer);

    SECTION("encode/decode int32") {
        pw.add_int32(1, 17);
        protozero::pbf item(buffer.data(), buffer.size());
        REQUIRE(item.next());
        REQUIRE(17 == item.get_int32());
    }

    SECTION("encode/decode uint32") {
        pw.add_uint32(1, 17U);
        protozero::pbf item(buffer.data(), buffer.size());
        REQUIRE(item.next());
        REQUIRE(17U == item.get_uint32());
    }

    SECTION("encode/decode uint64") {
        pw.add_uint64(1, (1ULL << 40));
        protozero::pbf item(buffer.data(), buffer.size());
        REQUIRE(item.next());
        REQUIRE((1ULL << 40) == item.get_uint64());
    }

    SECTION("short buffer while parsing varint") {
        pw.add_uint64(1, (1ULL << 40));
        buffer.resize(buffer.size() - 1); // "remove" last byte from buffer
        protozero::pbf item(buffer.data(), buffer.size());
        REQUIRE(item.next());
        REQUIRE_THROWS_AS(item.get_uint64(), protozero::pbf::end_of_buffer_exception);
    }

    SECTION("data corruption in buffer while parsing varint)") {
        pw.add_uint64(1, (1ULL << 20));
        buffer[buffer.size() - 1] += 0x80; // pretend the varint goes on
        protozero::pbf item(buffer.data(), buffer.size());
        REQUIRE(item.next());
        REQUIRE_THROWS_AS(item.get_uint64(), protozero::pbf::end_of_buffer_exception);
    }

    SECTION("data corruption in buffer while parsing varint (max length varint)") {
        pw.add_uint64(1, std::numeric_limits<uint64_t>::max());
        buffer[buffer.size() - 1] += 0x80; // pretend the varint goes on
        protozero::pbf item(buffer.data(), buffer.size());
        REQUIRE(item.next());
        REQUIRE_THROWS_AS(item.get_uint64(), protozero::pbf::varint_too_long_exception);
    }

}

