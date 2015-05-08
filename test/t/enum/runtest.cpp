
#include <test.hpp>

TEST_CASE("enum") {

    SECTION("zero") {
        std::string buffer = get_file_data("test/t/enum/data-black.pbf");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.get_enum() == 0L);
        REQUIRE(!item.next());
    }

    SECTION("positive") {
        std::string buffer = get_file_data("test/t/enum/data-blue.pbf");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.get_enum() == 3L);
        REQUIRE(!item.next());
    }

}

