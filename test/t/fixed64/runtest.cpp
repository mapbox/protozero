
#include <test.hpp>

TEST_CASE("fixed64") {

    SECTION("positive") {
        std::string buffer = get_file_data("test/t/fixed64/data1.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        while (item.next()) {
            int64_t data = item.fixed<int64_t>();
            REQUIRE(data == (1ll << 40));
        }
    }

    SECTION("negative") {
        std::string buffer = get_file_data("test/t/fixed64/data2.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        while (item.next()) {
            int64_t data = item.fixed<int64_t>();
            REQUIRE(data == - (1ll << 42));
        }
    }

}

