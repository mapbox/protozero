
#include <test.hpp>

TEST_CASE("double") {

    SECTION("positive") {
        std::string buffer = get_file_data("test/t/double/data1.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.float64() == 4.893);
        REQUIRE(!item.next());
    }

    SECTION("negative") {
        std::string buffer = get_file_data("test/t/double/data2.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.float64() == -9232.33);
        REQUIRE(!item.next());
    }

}

