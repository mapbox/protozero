
#include <test.hpp>

TEST_CASE("read fixed32 field") {

    SECTION("zero") {
        std::string buffer = load_data("fixed32/data-zero");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.get_fixed32() == 0UL);
        REQUIRE(!item.next());
    }

    SECTION("max-uint") {
        std::string buffer = load_data("fixed32/data-max-uint");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.get_fixed32() == std::numeric_limits<uint32_t>::max());
        REQUIRE(!item.next());
    }

    SECTION("min-uint") {
        std::string buffer = load_data("fixed32/data-min-uint");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.get_fixed32() == std::numeric_limits<uint32_t>::min());
        REQUIRE(!item.next());
    }

    SECTION("end_of_buffer") {
        std::string buffer = load_data("fixed32/data-min-uint");

        for (size_t i=1; i < buffer.size(); ++i) {
            mapbox::util::pbf item(buffer.data(), i);
            REQUIRE(item.next());
            REQUIRE_THROWS_AS(item.get_fixed32(), mapbox::util::pbf::end_of_buffer_exception);
        }
    }

    SECTION("check assert on varint/string access") {
        std::string buffer = load_data("fixed32/data-zero");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE_THROWS_AS(item.get_string(), assert_error);
    }

    SECTION("assert detecting tag==0") {
        std::string buffer = load_data("fixed32/data-zero");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE_THROWS_AS(item.get_fixed32(), assert_error);
        REQUIRE(item.next());
        REQUIRE(item.get_fixed32() == 0UL);
        REQUIRE_THROWS(item.get_fixed32());
        REQUIRE(!item.next());
    }

    SECTION("skip") {
        std::string buffer = load_data("fixed32/data-zero");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE_THROWS_AS(item.skip(), assert_error);
        REQUIRE(item.next());
        item.skip();
        REQUIRE_THROWS(item.skip());
        REQUIRE(!item.next());
    }

}

TEST_CASE("write fixed32 field") {

    std::string buffer;
    mapbox::util::pbf_writer pw(buffer);

    SECTION("zero") {
        pw.add_fixed32(1, 0);
        REQUIRE(buffer == load_data("fixed32/data-zero"));
    }

    SECTION("max-uint") {
        pw.add_fixed32(1, std::numeric_limits<uint32_t>::max());
        REQUIRE(buffer == load_data("fixed32/data-max-uint"));
    }

    SECTION("min-uint") {
        pw.add_fixed32(1, std::numeric_limits<uint32_t>::min());
        REQUIRE(buffer == load_data("fixed32/data-min-uint"));
    }

}

