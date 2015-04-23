
#include <test.hpp>

TEST_CASE("fixed32") {

    SECTION("positive") {
        std::string buffer = get_file_data("test/t/fixed32/data1.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        while (item.next()) {
            REQUIRE(item.fixed<uint32_t>() == 999);
        }
    }

    SECTION("negative") {
        std::string buffer = get_file_data("test/t/fixed32/data2.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        while (item.next()) {
            REQUIRE(item.fixed<uint32_t>() == -189);
        }
    }

}

