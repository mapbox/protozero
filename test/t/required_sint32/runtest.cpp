
#include <test.hpp>

TEST_CASE("required_sint32") {

    SECTION("positive") {
        std::string buffer = get_file_data("test/t/required_sint32/data1.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        while (item.next()) {
            REQUIRE(item.svarint() == 17);
        }
    }

    SECTION("negativ") {
        std::string buffer = get_file_data("test/t/required_sint32/data2.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        while (item.next()) {
            REQUIRE(item.svarint() == -1);
        }
    }

}

