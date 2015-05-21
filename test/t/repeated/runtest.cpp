
#include <test.hpp>

TEST_CASE("repeated") {

    SECTION("empty") {
        std::string buffer = get_file_data("test/t/repeated/data-empty.pbf");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(!item.next());
    }

    SECTION("one") {
        std::string buffer = get_file_data("test/t/repeated/data-one.pbf");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.get_int32() == 0L);
        REQUIRE(!item.next());
    }

    SECTION("many") {
        std::string buffer = get_file_data("test/t/repeated/data-many.pbf");

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
        std::string buffer = get_file_data("test/t/repeated/data-one.pbf");

        for (size_t i=1; i < buffer.size(); ++i) {
            mapbox::util::pbf item(buffer.data(), i);
            REQUIRE(item.next());
            REQUIRE_THROWS_AS(item.get_int32(), mapbox::util::pbf::end_of_buffer_exception);
        }
    }

}

TEST_CASE("write repeated") {

    SECTION("one") {
        std::string buffer = get_file_data("test/t/repeated/data-one.pbf");

        std::string wbuffer;
        mapbox::util::pbf_writer pw(wbuffer);
        pw.add_int32(1, 0L);

        REQUIRE(buffer == wbuffer);
    }

    SECTION("many") {
        std::string buffer = get_file_data("test/t/repeated/data-many.pbf");

        std::string wbuffer;
        mapbox::util::pbf_writer pw(wbuffer);
        pw.add_int32(1, 0L);
        pw.add_int32(1, 1L);
        pw.add_int32(1, -1L);
        pw.add_int32(1, std::numeric_limits<int32_t>::max());
        pw.add_int32(1, std::numeric_limits<int32_t>::min());

        REQUIRE(buffer == wbuffer);
    }

}

