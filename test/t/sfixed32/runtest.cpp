
#include <test.hpp>

TEST_CASE("sfixed32") {

    SECTION("zero") {
        std::string buffer = get_file_data("test/t/sfixed32/data-zero.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.sfixed32() == 0);
        REQUIRE(!item.next());
    }

    SECTION("max-int") {
        std::string buffer = get_file_data("test/t/sfixed32/data-max-int.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.sfixed32() == std::numeric_limits<int32_t>::max());
        REQUIRE(!item.next());
    }

    SECTION("min-int") {
        std::string buffer = get_file_data("test/t/sfixed32/data-min-int.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.sfixed32() == std::numeric_limits<int32_t>::min());
        REQUIRE(!item.next());
    }

}

