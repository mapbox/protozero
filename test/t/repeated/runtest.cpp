
#include <test.hpp>

TEST_CASE("read repeated fields") {

    SECTION("empty") {
        std::string buffer = load_data("repeated/data-empty");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(!item.next());
    }

    SECTION("one") {
        std::string buffer = load_data("repeated/data-one");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.get_int32() == 0L);
        REQUIRE(!item.next());
    }

    SECTION("many") {
        std::string buffer = load_data("repeated/data-many");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.get_int32() == 0L);

        REQUIRE(item.next());
        REQUIRE(item.get_int32() == 1L);

        REQUIRE(item.next());
        REQUIRE(item.get_int32() == -1L);

        REQUIRE(item.next());
        REQUIRE(item.get_int32() == std::numeric_limits<int32_t>::max());

        REQUIRE(item.next());
        REQUIRE(item.get_int32() == std::numeric_limits<int32_t>::min());

        REQUIRE(!item.next());
    }

    SECTION("end_of_buffer") {
        std::string buffer = load_data("repeated/data-one");

        for (size_t i=1; i < buffer.size(); ++i) {
            mapbox::util::pbf item(buffer.data(), i);
            REQUIRE(item.next());
            REQUIRE_THROWS_AS(item.get_int32(), mapbox::util::pbf::end_of_buffer_exception);
        }
    }

}

TEST_CASE("write repeated fields") {

    std::string buffer;
    mapbox::util::pbf_writer pw(buffer);

    SECTION("one") {
        pw.add_int32(1, 0L);
        REQUIRE(buffer == load_data("repeated/data-one"));
    }

    SECTION("many") {
        pw.add_int32(1, 0L);
        pw.add_int32(1, 1L);
        pw.add_int32(1, -1L);
        pw.add_int32(1, std::numeric_limits<int32_t>::max());
        pw.add_int32(1, std::numeric_limits<int32_t>::min());

        REQUIRE(buffer == load_data("repeated/data-many"));
    }

}

