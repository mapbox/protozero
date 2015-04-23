
#include <test.hpp>

TEST_CASE("float") {

    SECTION("positive") {
        std::string buffer = get_file_data("test/t/float/data1.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        while (item.next()) {
            REQUIRE(item.float32() == 5.34f);
        }
    }

    SECTION("negative") {
        std::string buffer = get_file_data("test/t/float/data2.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        while (item.next()) {
            REQUIRE(item.float32() == -1.71f);
        }
    }

}

