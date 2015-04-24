
#include <test.hpp>

TEST_CASE("sfixed64") {

    SECTION("zero") {
        std::string buffer = get_file_data("test/t/sfixed64/data-zero.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.sfixed64() == 0ll);
        REQUIRE(!item.next());
    }

    SECTION("max-int") {
        std::string buffer = get_file_data("test/t/sfixed64/data-max-int.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.sfixed64() == std::numeric_limits<int64_t>::max());
        REQUIRE(!item.next());
    }

    SECTION("min-int") {
        std::string buffer = get_file_data("test/t/sfixed64/data-min-int.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.sfixed64() == std::numeric_limits<int64_t>::min());
        REQUIRE(!item.next());
    }

}

