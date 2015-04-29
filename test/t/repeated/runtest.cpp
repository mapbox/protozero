
#include <test.hpp>

TEST_CASE("repeated") {

    SECTION("empty") {
        std::string buffer = get_file_data("test/t/repeated/data-empty.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(!item.next());
    }

    SECTION("one") {
        std::string buffer = get_file_data("test/t/repeated/data-one.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.varint<int32_t>() == 0l);
        REQUIRE(!item.next());
    }

    SECTION("many") {
        std::string buffer = get_file_data("test/t/repeated/data-many.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.varint<int32_t>() == 0l);

        REQUIRE(item.next());
        REQUIRE(item.varint<int32_t>() == 1l);

        REQUIRE(item.next());
        REQUIRE(item.varint<int32_t>() == -1l);

        REQUIRE(item.next());
        REQUIRE(item.varint<int32_t>() == std::numeric_limits<int32_t>::max());

        REQUIRE(item.next());
        REQUIRE(item.varint<int32_t>() == std::numeric_limits<int32_t>::min());

        REQUIRE(!item.next());
    }

}

