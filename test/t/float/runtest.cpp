
#include <test.hpp>

TEST_CASE("float") {

    SECTION("zero") {
        std::string buffer = get_file_data("test/t/float/data-zero.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.float32() == 0.0f);
        REQUIRE(!item.next());
    }

    SECTION("positive") {
        std::string buffer = get_file_data("test/t/float/data-pos.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.float32() == 5.34f);
        REQUIRE(!item.next());
    }

    SECTION("negative") {
        std::string buffer = get_file_data("test/t/float/data-neg.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.float32() == -1.71f);
        REQUIRE(!item.next());
    }

}

