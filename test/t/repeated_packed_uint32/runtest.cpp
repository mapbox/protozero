
#include <test.hpp>

TEST_CASE("repeated_packed_uint32") {

    SECTION("empty") {
        std::string buffer = get_file_data("test/t/repeated_packed_uint32/data-empty.pbf");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(!item.next());
    }

    SECTION("one") {
        std::string buffer = get_file_data("test/t/repeated_packed_uint32/data-one.pbf");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        auto it_pair = item.packed_uint32();
        REQUIRE(!item.next());

        REQUIRE(*it_pair.first == 17UL);
        REQUIRE(++it_pair.first == it_pair.second);
    }

    SECTION("many") {
        std::string buffer = get_file_data("test/t/repeated_packed_uint32/data-many.pbf");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        auto it_pair = item.packed_uint32();
        REQUIRE(!item.next());

        auto it = it_pair.first;
        REQUIRE(*it++ == 17UL);
        REQUIRE(*it++ ==  0UL);
        REQUIRE(*it++ ==  1UL);
        REQUIRE(*it++ == std::numeric_limits<uint32_t>::max());
        REQUIRE(it == it_pair.second);
    }

    SECTION("end_of_buffer") {
        std::string buffer = get_file_data("test/t/repeated_packed_uint32/data-many.pbf");

        for (size_t i=1; i < buffer.size(); ++i) {
            mapbox::util::pbf item(buffer.data(), i);
            REQUIRE(item.next());
            REQUIRE_THROWS_AS(item.packed_uint32(), mapbox::util::pbf::end_of_buffer_exception);
        }
    }

}

TEST_CASE("write repeated_packed_uint32") {

    SECTION("empty") {
        std::string buffer = get_file_data("test/t/repeated_packed_uint32/data-empty.pbf");

        std::string wbuffer;
        mapbox::util::pbf_writer pw(wbuffer);
        const uint32_t* dummy = 0;
        pw.add_packed_uint32(1, dummy, dummy);

        REQUIRE(buffer == wbuffer);
    }

    SECTION("one") {
        std::string buffer = get_file_data("test/t/repeated_packed_uint32/data-one.pbf");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        uint32_t data[] = { 17UL };

        std::string wbuffer;
        mapbox::util::pbf_writer pw(wbuffer);
        pw.add_packed_uint32(1, std::begin(data), std::end(data));

        REQUIRE(buffer == wbuffer);
    }

    SECTION("many") {
        std::string buffer = get_file_data("test/t/repeated_packed_uint32/data-many.pbf");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        uint32_t data[] = { 17UL, 0UL, 1UL, std::numeric_limits<uint32_t>::max() };

        std::string wbuffer;
        mapbox::util::pbf_writer pw(wbuffer);
        pw.add_packed_uint32(1, std::begin(data), std::end(data));

        REQUIRE(buffer == wbuffer);
    }

}

