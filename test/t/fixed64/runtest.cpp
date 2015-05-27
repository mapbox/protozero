
#include <test.hpp>

TEST_CASE("read fixed64 field") {

    SECTION("zero") {
        std::string buffer = load_data("fixed64/data-zero");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.get_fixed64() == 0ULL);
        REQUIRE(!item.next());
    }

    SECTION("max-uint") {
        std::string buffer = load_data("fixed64/data-max-uint");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.get_fixed64() == std::numeric_limits<uint64_t>::max());
        REQUIRE(!item.next());
    }

    SECTION("min-uint") {
        std::string buffer = load_data("fixed64/data-min-uint");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.get_fixed64() == std::numeric_limits<uint64_t>::min());
        REQUIRE(!item.next());
    }

    SECTION("end_of_buffer") {
        std::string buffer = load_data("fixed64/data-min-uint");

        for (size_t i=1; i < buffer.size(); ++i) {
            mapbox::util::pbf item(buffer.data(), i);
            REQUIRE(item.next());
            REQUIRE_THROWS_AS(item.get_fixed64(), mapbox::util::pbf::end_of_buffer_exception);
        }
    }

}

TEST_CASE("write fixed64 field") {

    std::string buffer;
    mapbox::util::pbf_writer pw(buffer);

    SECTION("zero") {
        pw.add_fixed64(1, 0ULL);
        REQUIRE(buffer == load_data("fixed64/data-zero"));
    }

    SECTION("max-uint") {
        pw.add_fixed64(1, std::numeric_limits<uint64_t>::max());
        REQUIRE(buffer == load_data("fixed64/data-max-uint"));
    }

    SECTION("min-uint") {
        pw.add_fixed64(1, std::numeric_limits<uint64_t>::min());
        REQUIRE(buffer == load_data("fixed64/data-min-uint"));
    }

}

