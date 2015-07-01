
#include <test.hpp>

TEST_CASE("read repeated packed fixed64 field") {

    SECTION("empty") {
        std::string buffer = load_data("repeated_packed_fixed64/data-empty");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(!item.next());
    }

    SECTION("one") {
        std::string buffer = load_data("repeated_packed_fixed64/data-one");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        auto it_pair = item.get_packed_fixed64();
        REQUIRE(!item.next());

        REQUIRE(*it_pair.first == 17ULL);
        REQUIRE(++it_pair.first == it_pair.second);
    }

    SECTION("many") {
        std::string buffer = load_data("repeated_packed_fixed64/data-many");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        auto it_pair = item.get_packed_fixed64();
        REQUIRE(!item.next());

        auto it = it_pair.first;
        REQUIRE(*it++ == 17ULL);
        REQUIRE(*it++ ==  0ULL);
        REQUIRE(*it++ ==  1ULL);
        REQUIRE(*it++ == std::numeric_limits<uint64_t>::max());
        REQUIRE(it == it_pair.second);
    }

    SECTION("end_of_buffer") {
        std::string buffer = load_data("repeated_packed_fixed64/data-many");

        for (size_t i=1; i < buffer.size(); ++i) {
            mapbox::util::pbf item(buffer.data(), i);
            REQUIRE(item.next());
            REQUIRE_THROWS_AS(item.get_packed_fixed64(), mapbox::util::pbf::end_of_buffer_exception);
        }
    }

}

TEST_CASE("write repeated packed fixed64 field") {

    std::string buffer;
    mapbox::util::pbf_writer pw(buffer);

    SECTION("empty") {
        uint64_t data[] = { 17ULL };
        pw.add_packed_fixed64(1, std::begin(data), std::begin(data) /* !!!! */);

        REQUIRE(buffer == load_data("repeated_packed_fixed64/data-empty"));
    }

    SECTION("one") {
        uint64_t data[] = { 17ULL };
        pw.add_packed_fixed64(1, std::begin(data), std::end(data));

        REQUIRE(buffer == load_data("repeated_packed_fixed64/data-one"));
    }

    SECTION("many") {
        uint64_t data[] = { 17ULL, 0ULL, 1ULL, std::numeric_limits<uint64_t>::max() };
        pw.add_packed_fixed64(1, std::begin(data), std::end(data));

        REQUIRE(buffer == load_data("repeated_packed_fixed64/data-many"));
    }

}

