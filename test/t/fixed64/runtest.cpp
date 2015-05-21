
#include <test.hpp>

TEST_CASE("fixed64") {

    SECTION("zero") {
        std::string buffer = get_file_data("test/t/fixed64/data-zero.pbf");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.get_fixed64() == 0ULL);
        REQUIRE(!item.next());
    }

    SECTION("max-uint") {
        std::string buffer = get_file_data("test/t/fixed64/data-max-uint.pbf");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.get_fixed64() == std::numeric_limits<uint64_t>::max());
        REQUIRE(!item.next());
    }

    SECTION("min-uint") {
        std::string buffer = get_file_data("test/t/fixed64/data-min-uint.pbf");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.get_fixed64() == std::numeric_limits<uint64_t>::min());
        REQUIRE(!item.next());
    }

    SECTION("end_of_buffer") {
        std::string buffer = get_file_data("test/t/fixed64/data-min-uint.pbf");

        for (size_t i=1; i < buffer.size(); ++i) {
            mapbox::util::pbf item(buffer.data(), i);
            REQUIRE(item.next());
            REQUIRE_THROWS_AS(item.get_fixed64(), mapbox::util::pbf::end_of_buffer_exception);
        }
    }

}

TEST_CASE("write fixed64") {

    SECTION("zero") {
        std::string buffer = get_file_data("test/t/fixed64/data-zero.pbf");

        std::string wbuffer;
        mapbox::util::pbf_writer pw(wbuffer);
        pw.add_fixed64(1, 0ULL);

        REQUIRE(buffer == wbuffer);
    }

    SECTION("max-uint") {
        std::string buffer = get_file_data("test/t/fixed64/data-max-uint.pbf");

        std::string wbuffer;
        mapbox::util::pbf_writer pw(wbuffer);
        pw.add_fixed64(1, std::numeric_limits<uint64_t>::max());

        REQUIRE(buffer == wbuffer);
    }

    SECTION("min-uint") {
        std::string buffer = get_file_data("test/t/fixed64/data-min-uint.pbf");

        std::string wbuffer;
        mapbox::util::pbf_writer pw(wbuffer);
        pw.add_fixed64(1, std::numeric_limits<uint64_t>::min());

        REQUIRE(buffer == wbuffer);
    }

}

