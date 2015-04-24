
#include <test.hpp>

TEST_CASE("sint32") {

    SECTION("positive") {
        std::string buffer = get_file_data("test/t/sint32/data1.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.svarint<int32_t>() == 17);
        REQUIRE(!item.next());
    }

    SECTION("negative") {
        std::string buffer = get_file_data("test/t/sint32/data2.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.svarint<int32_t>() == -1);
        REQUIRE(!item.next());
    }

}

