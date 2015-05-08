
#include <test.hpp>

TEST_CASE("fixed32") {

    SECTION("zero") {
        std::string buffer = get_file_data("test/t/fixed32/data-zero.pbf");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.get_fixed32() == 0UL);
        REQUIRE(!item.next());
    }

    SECTION("max-uint") {
        std::string buffer = get_file_data("test/t/fixed32/data-max-uint.pbf");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.get_fixed32() == std::numeric_limits<uint32_t>::max());
        REQUIRE(!item.next());
    }

    SECTION("min-uint") {
        std::string buffer = get_file_data("test/t/fixed32/data-min-uint.pbf");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.get_fixed32() == std::numeric_limits<uint32_t>::min());
        REQUIRE(!item.next());
    }

    SECTION("end_of_buffer") {
        std::string buffer = get_file_data("test/t/fixed32/data-min-uint.pbf");

        for (size_t i=1; i < buffer.size(); ++i) {
            mapbox::util::pbf item(buffer.data(), i);
            REQUIRE(item.next());
            REQUIRE_THROWS_AS(item.get_fixed32(), mapbox::util::pbf::end_of_buffer_exception);
        }
    }

    SECTION("check assert on varint/string access") {
        std::string buffer = get_file_data("test/t/fixed32/data-zero.pbf");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE_THROWS_AS(item.get_string(), assert_error);
    }

}

