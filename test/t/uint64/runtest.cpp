
#include <test.hpp>

TEST_CASE("uint64") {

    SECTION("zero") {
        std::string buffer = get_file_data("test/t/uint64/data-zero.pbf");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.get_uint64() == 0ul);
        REQUIRE(!item.next());
    }

    SECTION("positive") {
        std::string buffer = get_file_data("test/t/uint64/data-pos.pbf");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.get_uint64() == 1ul);
        REQUIRE(!item.next());
    }

    SECTION("max") {
        std::string buffer = get_file_data("test/t/uint64/data-max.pbf");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.get_uint64() == std::numeric_limits<uint64_t>::max());
        REQUIRE(!item.next());
    }

    SECTION("end_of_buffer") {
        std::string buffer = get_file_data("test/t/uint64/data-max.pbf");

        for (size_t i=1; i < buffer.size(); ++i) {
            mapbox::util::pbf item(buffer.data(), i);
            REQUIRE(item.next());
            REQUIRE_THROWS_AS(item.get_uint64(), mapbox::util::pbf::end_of_buffer_exception);
        }
    }

}

TEST_CASE("write uint64") {

    SECTION("zero") {
        std::string buffer = get_file_data("test/t/uint64/data-zero.pbf");

        std::string wbuffer;
        mapbox::util::pbf_writer pw(wbuffer);
        pw.add_uint64(1, 0UL);

        REQUIRE(buffer == wbuffer);
    }

    SECTION("positive") {
        std::string buffer = get_file_data("test/t/uint64/data-pos.pbf");

        std::string wbuffer;
        mapbox::util::pbf_writer pw(wbuffer);
        pw.add_uint64(1, 1UL);

        REQUIRE(buffer == wbuffer);
    }

    SECTION("max") {
        std::string buffer = get_file_data("test/t/uint64/data-max.pbf");

        std::string wbuffer;
        mapbox::util::pbf_writer pw(wbuffer);
        pw.add_uint64(1, std::numeric_limits<uint64_t>::max());

        REQUIRE(buffer == wbuffer);
    }

}

