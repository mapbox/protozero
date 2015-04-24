
#include <test.hpp>

TEST_CASE("double") {

    SECTION("zero") {
        std::string buffer = get_file_data("test/t/double/data-zero.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.float64() == 0.0);
        REQUIRE(!item.next());
    }

    SECTION("positive") {
        std::string buffer = get_file_data("test/t/double/data-pos.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.float64() == 4.893);
        REQUIRE(!item.next());
    }

    SECTION("negative") {
        std::string buffer = get_file_data("test/t/double/data-neg.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.float64() == -9232.33);
        REQUIRE(!item.next());
    }

}

