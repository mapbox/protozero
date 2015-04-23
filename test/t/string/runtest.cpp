
#include <test.hpp>

TEST_CASE("string") {

    SECTION("positive") {
        std::string buffer = get_file_data("test/t/string/data1.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        while (item.next()) {
            REQUIRE(item.string() == "foobar");
        }
    }

    SECTION("negative") {
        std::string buffer = get_file_data("test/t/string/data2.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        while (item.next()) {
            REQUIRE(item.string() == "");
        }
    }

}

