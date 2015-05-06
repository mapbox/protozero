
#include <test.hpp>

TEST_CASE("sfixed32") {

    SECTION("zero") {
        std::string buffer = get_file_data("test/t/sfixed32/data-zero.pbf");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.sfixed32() == 0L);
        REQUIRE(!item.next());
    }

    SECTION("max-int") {
        std::string buffer = get_file_data("test/t/sfixed32/data-max-int.pbf");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.sfixed32() == std::numeric_limits<int32_t>::max());
        REQUIRE(!item.next());
    }

    SECTION("min-int") {
        std::string buffer = get_file_data("test/t/sfixed32/data-min-int.pbf");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.sfixed32() == std::numeric_limits<int32_t>::min());
        REQUIRE(!item.next());
    }

    SECTION("end_of_buffer") {
        std::string buffer = get_file_data("test/t/sfixed32/data-min-int.pbf");

        for (size_t i=1; i < buffer.size(); ++i) {
            mapbox::util::pbf item(buffer.data(), i);
            REQUIRE(item.next());
            REQUIRE_THROWS_AS(item.sfixed32(), mapbox::util::pbf::end_of_buffer_exception);
        }
    }

}

