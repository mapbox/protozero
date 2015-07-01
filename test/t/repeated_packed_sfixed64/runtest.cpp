
#include <test.hpp>

TEST_CASE("read repeated packed sfixed64 field") {

    SECTION("empty") {
        std::string buffer = load_data("repeated_packed_sfixed64/data-empty");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(!item.next());
    }

    SECTION("one") {
        std::string buffer = load_data("repeated_packed_sfixed64/data-one");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        auto it_pair = item.get_packed_sfixed64();
        REQUIRE(!item.next());

        REQUIRE(*it_pair.first == 17LL);
        REQUIRE(++it_pair.first == it_pair.second);
    }

    SECTION("many") {
        std::string buffer = load_data("repeated_packed_sfixed64/data-many");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        auto it_pair = item.get_packed_sfixed64();
        REQUIRE(!item.next());

        auto it = it_pair.first;
        REQUIRE(*it++ == 17LL);
        REQUIRE(*it++ ==  0LL);
        REQUIRE(*it++ ==  1LL);
        REQUIRE(*it++ == -1LL);
        REQUIRE(*it++ == std::numeric_limits<int64_t>::max());
        REQUIRE(*it++ == std::numeric_limits<int64_t>::min());
        REQUIRE(it == it_pair.second);
    }

    SECTION("end_of_buffer") {
        std::string buffer = load_data("repeated_packed_sfixed64/data-many");

        for (size_t i=1; i < buffer.size(); ++i) {
            mapbox::util::pbf item(buffer.data(), i);
            REQUIRE(item.next());
            REQUIRE_THROWS_AS(item.get_packed_sfixed64(), mapbox::util::pbf::end_of_buffer_exception);
        }
    }

}

TEST_CASE("write repeated packed sfixed64 field") {

    std::string buffer;
    mapbox::util::pbf_writer pw(buffer);

    SECTION("empty") {
        int64_t data[] = { 17L };
        pw.add_packed_sfixed64(1, std::begin(data), std::begin(data) /* !!!! */);

        REQUIRE(buffer == load_data("repeated_packed_sfixed64/data-empty"));
    }

    SECTION("one") {
        int64_t data[] = { 17L };
        pw.add_packed_sfixed64(1, std::begin(data), std::end(data));

        REQUIRE(buffer == load_data("repeated_packed_sfixed64/data-one"));
    }

    SECTION("many") {
        int64_t data[] = { 17L, 0L, 1L, -1L, std::numeric_limits<int64_t>::max(), std::numeric_limits<int64_t>::min() };
        pw.add_packed_sfixed64(1, std::begin(data), std::end(data));

        REQUIRE(buffer == load_data("repeated_packed_sfixed64/data-many"));
    }

}

