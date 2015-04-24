
#include <test.hpp>

TEST_CASE("string") {

    SECTION("empty") {
        std::string buffer = get_file_data("test/t/string/data-empty.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.string() == "");
        REQUIRE(!item.next());
    }

    SECTION("one") {
        std::string buffer = get_file_data("test/t/string/data-one.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.string() == "x");
        REQUIRE(!item.next());
    }

    SECTION("string") {
        std::string buffer = get_file_data("test/t/string/data-string.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.string() == "foobar");
        REQUIRE(!item.next());
    }

}

