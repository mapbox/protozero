
#include <test.hpp>

TEST_CASE("sint64") {

    SECTION("zero") {
        std::string buffer = get_file_data("test/t/sint64/data-zero.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.svarint<int64_t>() == 0l);
        REQUIRE(!item.next());
    }

    SECTION("positive") {
        std::string buffer = get_file_data("test/t/sint64/data-pos.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.svarint<int64_t>() == 1l);
        REQUIRE(!item.next());
    }

    SECTION("negative") {
        std::string buffer = get_file_data("test/t/sint64/data-neg.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.svarint<int64_t>() == -1l);
        REQUIRE(!item.next());
    }

    SECTION("max") {
        std::string buffer = get_file_data("test/t/sint64/data-max.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.svarint<int64_t>() == std::numeric_limits<int64_t>::max());
        REQUIRE(!item.next());
    }

    SECTION("min") {
        std::string buffer = get_file_data("test/t/sint64/data-min.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.svarint<int64_t>() == std::numeric_limits<int64_t>::min());
        REQUIRE(!item.next());
    }

}

