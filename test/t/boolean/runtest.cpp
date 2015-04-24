
#include <test.hpp>

TEST_CASE("boolean") {

    SECTION("false") {
        std::string buffer = get_file_data("test/t/boolean/data-false.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(!item.boolean());
        REQUIRE(!item.next());
    }

    SECTION("true") {
        std::string buffer = get_file_data("test/t/boolean/data-true.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.boolean());
        REQUIRE(!item.next());
    }

    SECTION("also true") {
        std::string buffer = get_file_data("test/t/boolean/data-also-true.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.boolean());
        REQUIRE(!item.next());
    }

    SECTION("still true") {
        std::string buffer = get_file_data("test/t/boolean/data-still-true.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.boolean());
        REQUIRE(!item.next());
    }

}

