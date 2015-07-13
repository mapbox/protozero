
#include <test.hpp>

TEST_CASE("read repeated packed uint32 field") {

    SECTION("empty") {
        std::string buffer = load_data("repeated_packed_uint32/data-empty");

        protozero::pbf item(buffer.data(), buffer.size());

        REQUIRE(!item.next());
    }

    SECTION("one") {
        std::string buffer = load_data("repeated_packed_uint32/data-one");

        protozero::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        auto it_pair = item.get_packed_uint32();
        REQUIRE(!item.next());

        REQUIRE(*it_pair.first == 17UL);
        REQUIRE(++it_pair.first == it_pair.second);
    }

    SECTION("many") {
        std::string buffer = load_data("repeated_packed_uint32/data-many");

        protozero::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        auto it_pair = item.get_packed_uint32();
        REQUIRE(!item.next());

        auto it = it_pair.first;
        REQUIRE(*it++ == 17UL);
        REQUIRE(*it++ ==  0UL);
        REQUIRE(*it++ ==  1UL);
        REQUIRE(*it++ == std::numeric_limits<uint32_t>::max());
        REQUIRE(it == it_pair.second);
    }

    SECTION("end_of_buffer") {
        std::string buffer = load_data("repeated_packed_uint32/data-many");

        for (size_t i=1; i < buffer.size(); ++i) {
            protozero::pbf item(buffer.data(), i);
            REQUIRE(item.next());
            REQUIRE_THROWS_AS(item.get_packed_uint32(), protozero::pbf::end_of_buffer_exception);
        }
    }

}

TEST_CASE("write repeated packed uint32 field") {

    std::string buffer;
    protozero::pbf_writer pw(buffer);

    SECTION("empty") {
        uint32_t data[] = { 17UL };
        pw.add_packed_uint32(1, std::begin(data), std::begin(data) /* !!!! */);

        REQUIRE(buffer == load_data("repeated_packed_uint32/data-empty"));
    }

    SECTION("one") {
        uint32_t data[] = { 17UL };
        pw.add_packed_uint32(1, std::begin(data), std::end(data));

        REQUIRE(buffer == load_data("repeated_packed_uint32/data-one"));
    }

    SECTION("many") {
        uint32_t data[] = { 17UL, 0UL, 1UL, std::numeric_limits<uint32_t>::max() };
        pw.add_packed_uint32(1, std::begin(data), std::end(data));

        REQUIRE(buffer == load_data("repeated_packed_uint32/data-many"));
    }

}

