
#include <test.hpp>

TEST_CASE("int32") {

    SECTION("positive") {
        std::string buffer = get_file_data("test/t/int32/data1.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.varint() == 17);
        REQUIRE(!item.next());
    }

    SECTION("negative") {
        std::string buffer = get_file_data("test/t/int32/data2.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.varint() == -1);
        REQUIRE(!item.next());
    }

}

