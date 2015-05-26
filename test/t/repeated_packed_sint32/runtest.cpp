
#include <test.hpp>

TEST_CASE("repeated_packed_sint32") {

    SECTION("empty") {
        std::string buffer = get_file_data("test/t/repeated_packed_sint32/data-empty.pbf");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(!item.next());
    }

    SECTION("one") {
        std::string buffer = get_file_data("test/t/repeated_packed_sint32/data-one.pbf");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        auto it_pair = item.packed_sint32();
        REQUIRE(!item.next());

        REQUIRE(*it_pair.first == 17L);
        REQUIRE(++it_pair.first == it_pair.second);
    }

    SECTION("many") {
        std::string buffer = get_file_data("test/t/repeated_packed_sint32/data-many.pbf");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        auto it_pair = item.packed_sint32();
        REQUIRE(!item.next());

        auto it = it_pair.first;
        REQUIRE(*it++ == 17L);
        REQUIRE(*it++ ==  0L);
        REQUIRE(*it++ ==  1L);
        REQUIRE(*it++ == -1L);
        REQUIRE(*it++ == std::numeric_limits<int32_t>::max());
        REQUIRE(*it++ == std::numeric_limits<int32_t>::min());
        REQUIRE(it == it_pair.second);
    }

    SECTION("end_of_buffer") {
        std::string buffer = get_file_data("test/t/repeated_packed_sint32/data-many.pbf");

        for (size_t i=1; i < buffer.size(); ++i) {
            mapbox::util::pbf item(buffer.data(), i);
            REQUIRE(item.next());
            REQUIRE_THROWS_AS(item.packed_sint32(), mapbox::util::pbf::end_of_buffer_exception);
        }
    }

}

TEST_CASE("write repeated_packed_sint32") {

    SECTION("empty") {
        std::string buffer = get_file_data("test/t/repeated_packed_sint32/data-empty.pbf");

        std::string wbuffer;
        mapbox::util::pbf_writer pw(wbuffer);
        const int32_t* dummy = 0;
        pw.add_packed_sint32(1, dummy, dummy);

        REQUIRE(buffer == wbuffer);
    }

    SECTION("one") {
        std::string buffer = get_file_data("test/t/repeated_packed_sint32/data-one.pbf");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        int32_t data[] = { 17L };

        std::string wbuffer;
        mapbox::util::pbf_writer pw(wbuffer);
        pw.add_packed_sint32(1, std::begin(data), std::end(data));

        REQUIRE(buffer == wbuffer);
    }

    SECTION("many") {
        std::string buffer = get_file_data("test/t/repeated_packed_sint32/data-many.pbf");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        int32_t data[] = { 17L, 0L, 1L, -1L, std::numeric_limits<int32_t>::max(), std::numeric_limits<int32_t>::min() };

        std::string wbuffer;
        mapbox::util::pbf_writer pw(wbuffer);
        pw.add_packed_sint32(1, std::begin(data), std::end(data));

        REQUIRE(buffer == wbuffer);
    }

}

