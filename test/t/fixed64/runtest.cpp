
#include <test.hpp>

TEST_CASE("fixed64") {

    SECTION("zero") {
        std::string buffer = get_file_data("test/t/fixed64/data-zero.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        while (item.next()) {
            REQUIRE(item.fixed<int64_t>() == 0);
        }
    }

    SECTION("max-int") {
        std::string buffer = get_file_data("test/t/fixed64/data-max-int.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        while (item.next()) {
            REQUIRE(item.fixed<int64_t>() == std::numeric_limits<int64_t>::max());
        }
    }

    SECTION("min-int") {
        std::string buffer = get_file_data("test/t/fixed64/data-min-int.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        while (item.next()) {
            REQUIRE(item.fixed<int64_t>() == std::numeric_limits<int64_t>::min());
        }
    }

    SECTION("max-uint") {
        std::string buffer = get_file_data("test/t/fixed64/data-max-uint.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        while (item.next()) {
            REQUIRE(item.fixed<uint64_t>() == std::numeric_limits<uint64_t>::max());
        }
    }

    SECTION("min-uint") {
        std::string buffer = get_file_data("test/t/fixed64/data-min-uint.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        while (item.next()) {
            REQUIRE(item.fixed<uint64_t>() == std::numeric_limits<uint64_t>::min());
        }
    }

}

