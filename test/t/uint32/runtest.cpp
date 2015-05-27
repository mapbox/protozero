
#include <test.hpp>

TEST_CASE("read uint32 field") {

    SECTION("zero") {
        std::string buffer = load_data("uint32/data-zero");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.get_uint32() == 0ul);
        REQUIRE(!item.next());
    }

    SECTION("positive") {
        std::string buffer = load_data("uint32/data-pos");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.get_uint32() == 1ul);
        REQUIRE(!item.next());
    }

    SECTION("max") {
        std::string buffer = load_data("uint32/data-max");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.get_uint32() == std::numeric_limits<uint32_t>::max());
        REQUIRE(!item.next());
    }

    SECTION("end_of_buffer") {
        std::string buffer = load_data("uint32/data-max");

        for (size_t i=1; i < buffer.size(); ++i) {
            mapbox::util::pbf item(buffer.data(), i);
            REQUIRE(item.next());
            REQUIRE_THROWS_AS(item.get_uint32(), mapbox::util::pbf::end_of_buffer_exception);
        }
    }

}

TEST_CASE("write uint32 field") {

    std::string buffer;
    mapbox::util::pbf_writer pw(buffer);

    SECTION("zero") {
        pw.add_uint32(1, 0UL);
        REQUIRE(buffer == load_data("uint32/data-zero"));
    }

    SECTION("positive") {
        pw.add_uint32(1, 1UL);
        REQUIRE(buffer == load_data("uint32/data-pos"));
    }

    SECTION("max") {
        pw.add_uint32(1, std::numeric_limits<uint32_t>::max());
        REQUIRE(buffer == load_data("uint32/data-max"));
    }

}

