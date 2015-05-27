
#include <test.hpp>

TEST_CASE("read repeated packed uint64 field") {

    SECTION("empty") {
        std::string buffer = load_data("repeated_packed_uint64/data-empty");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(!item.next());
    }

    SECTION("one") {
        std::string buffer = load_data("repeated_packed_uint64/data-one");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        auto it_pair = item.packed_uint64();
        REQUIRE(!item.next());

        REQUIRE(*it_pair.first == 17ULL);
        REQUIRE(++it_pair.first == it_pair.second);
    }

    SECTION("many") {
        std::string buffer = load_data("repeated_packed_uint64/data-many");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        auto it_pair = item.packed_uint64();
        REQUIRE(!item.next());

        auto it = it_pair.first;
        REQUIRE(*it++ == 17ULL);
        REQUIRE(*it++ ==  0ULL);
        REQUIRE(*it++ ==  1ULL);
        REQUIRE(*it++ == std::numeric_limits<uint64_t>::max());
        REQUIRE(it == it_pair.second);
    }

    SECTION("end_of_buffer") {
        std::string buffer = load_data("repeated_packed_uint64/data-many");

        for (size_t i=1; i < buffer.size(); ++i) {
            mapbox::util::pbf item(buffer.data(), i);
            REQUIRE(item.next());
            REQUIRE_THROWS_AS(item.packed_uint64(), mapbox::util::pbf::end_of_buffer_exception);
        }
    }

}

TEST_CASE("write repeated packed uint64 field") {

    std::string buffer;
    mapbox::util::pbf_writer pw(buffer);

    SECTION("empty") {
        const uint64_t* dummy = 0;
        pw.add_packed_uint64(1, dummy, dummy);

        REQUIRE(buffer == load_data("repeated_packed_uint64/data-empty"));
    }

    SECTION("one") {
        uint64_t data[] = { 17UL };
        pw.add_packed_uint64(1, std::begin(data), std::end(data));

        REQUIRE(buffer == load_data("repeated_packed_uint64/data-one"));
    }

    SECTION("many") {
        uint64_t data[] = { 17UL, 0UL, 1UL, std::numeric_limits<uint64_t>::max() };
        pw.add_packed_uint64(1, std::begin(data), std::end(data));

        REQUIRE(buffer == load_data("repeated_packed_uint64/data-many"));
    }

}

