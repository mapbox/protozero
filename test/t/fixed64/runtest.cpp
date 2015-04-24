
#include <test.hpp>

TEST_CASE("fixed64") {

    SECTION("zero") {
        std::string buffer = get_file_data("test/t/fixed64/data-zero.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.fixed64() == 0ull);
        REQUIRE(!item.next());
    }

    SECTION("max-uint") {
        std::string buffer = get_file_data("test/t/fixed64/data-max-uint.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.fixed64() == std::numeric_limits<uint64_t>::max());
        REQUIRE(!item.next());
    }

    SECTION("min-uint") {
        std::string buffer = get_file_data("test/t/fixed64/data-min-uint.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.fixed64() == std::numeric_limits<uint64_t>::min());
        REQUIRE(!item.next());
    }

}

