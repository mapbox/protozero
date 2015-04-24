
#include <test.hpp>

TEST_CASE("uint32") {

    SECTION("zero") {
        std::string buffer = get_file_data("test/t/uint32/data-zero.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.varint<uint32_t>() == 0ul);
        REQUIRE(!item.next());
    }

    SECTION("positive") {
        std::string buffer = get_file_data("test/t/uint32/data-pos.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.varint<uint32_t>() == 1ul);
        REQUIRE(!item.next());
    }

    SECTION("max") {
        std::string buffer = get_file_data("test/t/uint32/data-max.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.varint<uint32_t>() == std::numeric_limits<uint32_t>::max());
        REQUIRE(!item.next());
    }

}

