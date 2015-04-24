
#include <test.hpp>

TEST_CASE("string") {

    SECTION("positive") {
        std::string buffer = get_file_data("test/t/string/data1.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.string() == "foobar");
        REQUIRE(!item.next());
    }

    SECTION("negative") {
        std::string buffer = get_file_data("test/t/string/data2.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.string() == "");
        REQUIRE(!item.next());
    }

}

