
#include <test.hpp>

TEST_CASE("fixed32") {

    SECTION("zero") {
        std::string buffer = get_file_data("test/t/fixed32/data-zero.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.fixed32() == 0);
        REQUIRE(!item.next());
    }

    SECTION("max-uint") {
        std::string buffer = get_file_data("test/t/fixed32/data-max-uint.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.fixed32() == std::numeric_limits<uint32_t>::max());
        REQUIRE(!item.next());
    }

    SECTION("min-uint") {
        std::string buffer = get_file_data("test/t/fixed32/data-min-uint.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.fixed32() == std::numeric_limits<uint32_t>::min());
        REQUIRE(!item.next());
    }

}

