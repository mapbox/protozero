
#include <test.hpp>

TEST_CASE("read repeated packed int32 field") {

    SECTION("empty") {
        const std::string buffer = load_data("repeated_packed_int32/data-empty");

        protozero::pbf_reader item(buffer);

        REQUIRE(!item.next());
    }

    SECTION("one") {
        const std::string buffer = load_data("repeated_packed_int32/data-one");

        protozero::pbf_reader item(buffer);

        REQUIRE(item.next());
        auto it_pair = item.get_packed_int32();
        REQUIRE(!item.next());

        REQUIRE(it_pair.first != it_pair.second);
        REQUIRE(*it_pair.first == 17L);
        REQUIRE(++it_pair.first == it_pair.second);
    }

    SECTION("many") {
        const std::string buffer = load_data("repeated_packed_int32/data-many");

        protozero::pbf_reader item(buffer);

        REQUIRE(item.next());
        auto it_pair = item.get_packed_int32();
        REQUIRE(!item.next());

        auto it = it_pair.first;
        REQUIRE(it != it_pair.second);
        REQUIRE(*it++ == 17L);
        REQUIRE(*it++ ==  0L);
        REQUIRE(*it++ ==  1L);
        REQUIRE(*it++ ==  -1L);
        REQUIRE(*it++ == std::numeric_limits<int32_t>::max());
        REQUIRE(*it++ == std::numeric_limits<int32_t>::min());
        REQUIRE(it == it_pair.second);
    }

    SECTION("end_of_buffer") {
        const std::string buffer = load_data("repeated_packed_int32/data-many");

        for (std::string::size_type i = 1; i < buffer.size(); ++i) {
            protozero::pbf_reader item(buffer.data(), i);
            REQUIRE(item.next());
            REQUIRE_THROWS_AS(item.get_packed_int32(), protozero::end_of_buffer_exception);
        }
    }

}

TEST_CASE("write repeated packed int32 field") {

    std::string buffer;
    protozero::pbf_writer pw(buffer);

    SECTION("empty") {
        int32_t data[] = { 17L };
        pw.add_packed_int32(1, std::begin(data), std::begin(data) /* !!!! */);

        REQUIRE(buffer == load_data("repeated_packed_int32/data-empty"));
    }

    SECTION("one") {
        int32_t data[] = { 17L };
        pw.add_packed_int32(1, std::begin(data), std::end(data));

        REQUIRE(buffer == load_data("repeated_packed_int32/data-one"));
    }

    SECTION("many") {
        int32_t data[] = { 17L, 0L, 1L, -1L, std::numeric_limits<int32_t>::max(), std::numeric_limits<int32_t>::min() };
        pw.add_packed_int32(1, std::begin(data), std::end(data));

        REQUIRE(buffer == load_data("repeated_packed_int32/data-many"));
    }

}

