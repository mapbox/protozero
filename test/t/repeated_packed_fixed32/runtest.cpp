
#include <test.hpp>

TEST_CASE("read repeated packed fixed32 field") {

    SECTION("empty") {
        std::string buffer = load_data("repeated_packed_fixed32/data-empty");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(!item.next());
    }

    SECTION("one") {
        std::string buffer = load_data("repeated_packed_fixed32/data-one");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        auto it_pair = item.packed_fixed32();
        REQUIRE(!item.next());

        REQUIRE(*it_pair.first == 17UL);
        REQUIRE(++it_pair.first == it_pair.second);
    }

    SECTION("many") {
        std::string buffer = load_data("repeated_packed_fixed32/data-many");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        auto it_pair = item.packed_fixed32();
        REQUIRE(!item.next());

        auto it = it_pair.first;
        REQUIRE(*it++ == 17UL);
        REQUIRE(*it++ ==  0UL);
        REQUIRE(*it++ ==  1UL);
        REQUIRE(*it++ == std::numeric_limits<uint32_t>::max());
        REQUIRE(it == it_pair.second);
    }

    SECTION("end_of_buffer") {
        std::string buffer = load_data("repeated_packed_fixed32/data-many");

        for (size_t i=1; i < buffer.size(); ++i) {
            mapbox::util::pbf item(buffer.data(), i);
            REQUIRE(item.next());
            REQUIRE_THROWS_AS(item.packed_fixed32(), mapbox::util::pbf::end_of_buffer_exception);
        }
    }

}

TEST_CASE("write repeated packed fixed32 field") {

    std::string buffer;
    mapbox::util::pbf_writer pw(buffer);

    SECTION("empty") {
        uint32_t data[] = { 17UL };
        pw.add_packed_fixed32(1, std::begin(data), std::begin(data) /* !!!! */);

        REQUIRE(buffer == load_data("repeated_packed_fixed32/data-empty"));
    }

    SECTION("one") {
        uint32_t data[] = { 17UL };
        pw.add_packed_fixed32(1, std::begin(data), std::end(data));

        REQUIRE(buffer == load_data("repeated_packed_fixed32/data-one"));
    }

    SECTION("many") {
        uint32_t data[] = { 17UL, 0UL, 1UL, std::numeric_limits<uint32_t>::max() };
        pw.add_packed_fixed32(1, std::begin(data), std::end(data));

        REQUIRE(buffer == load_data("repeated_packed_fixed32/data-many"));
    }

}

TEST_CASE("write from different types of iterators") {

    std::string buffer;
    mapbox::util::pbf_writer pw(buffer);

    SECTION("from uint16_t") {
        uint16_t data[] = { 1, 4, 9, 16, 25 };

        pw.add_packed_fixed32(1, std::begin(data), std::end(data));
    }

    SECTION("from uint16_t with std::copy") {
        uint16_t data[] = { 1, 4, 9, 16, 25 };

        mapbox::util::pbf_appender<uint32_t> a(pw, 1, uint32_t(std::distance(std::begin(data), std::end(data))));
        std::copy(std::begin(data), std::end(data), a);
    }

    SECTION("from string") {
        std::string data = "1 4 9 16 25";
        std::stringstream sdata(data);

        std::istream_iterator<uint32_t> eod;
        std::istream_iterator<uint32_t> it(sdata);

        pw.add_packed_fixed32(1, it, eod);
    }

    mapbox::util::pbf item(buffer.data(), buffer.size());

    REQUIRE(item.next());
    auto it_pair = item.packed_fixed32();
    REQUIRE(!item.next());
    REQUIRE(std::distance(it_pair.first, it_pair.second) == 5);

    auto i = it_pair.first;
    REQUIRE(*i++ ==  1);
    REQUIRE(*i++ ==  4);
    REQUIRE(*i++ ==  9);
    REQUIRE(*i++ == 16);
    REQUIRE(*i++ == 25);

}

