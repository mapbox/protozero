
#include <test.hpp>

TEST_CASE("uint64") {

    SECTION("zero") {
        std::string buffer = get_file_data("test/t/uint64/data-zero.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.varint<uint64_t>() == 0ul);
        REQUIRE(!item.next());
    }

    SECTION("positive") {
        std::string buffer = get_file_data("test/t/uint64/data-pos.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.varint<uint64_t>() == 1ul);
        REQUIRE(!item.next());
    }

    SECTION("max") {
        std::string buffer = get_file_data("test/t/uint64/data-max.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.varint<uint64_t>() == std::numeric_limits<uint64_t>::max());
        REQUIRE(!item.next());
    }

}

