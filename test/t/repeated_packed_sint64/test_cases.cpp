
#include <test.hpp>

TEST_CASE("read repeated packed sint64 field") {

    SECTION("empty") {
        const std::string buffer = load_data("repeated_packed_sint64/data-empty");

        protozero::pbf_reader item(buffer);

        REQUIRE(!item.next());
    }

    SECTION("one") {
        const std::string buffer = load_data("repeated_packed_sint64/data-one");

        protozero::pbf_reader item(buffer);

        REQUIRE(item.next());
        auto it_range = item.get_packed_sint64();
        REQUIRE(!item.next());

        REQUIRE(*it_range.begin() == 17LL);
        REQUIRE(std::next(it_range.begin()) == it_range.end());
    }

    SECTION("many") {
        const std::string buffer = load_data("repeated_packed_sint64/data-many");

        protozero::pbf_reader item(buffer);

        REQUIRE(item.next());
        auto it_range = item.get_packed_sint64();
        REQUIRE(!item.next());

        auto it = it_range.begin();
        REQUIRE(*it++ == 17LL);
        REQUIRE(*it++ ==  0LL);
        REQUIRE(*it++ ==  1LL);
        REQUIRE(*it++ == -1LL);
        REQUIRE(*it++ == std::numeric_limits<int64_t>::max());
        REQUIRE(*it++ == std::numeric_limits<int64_t>::min());
        REQUIRE(it == it_range.end());
    }

    SECTION("end_of_buffer") {
        const std::string buffer = load_data("repeated_packed_sint64/data-many");

        for (std::string::size_type i = 1; i < buffer.size(); ++i) {
            protozero::pbf_reader item(buffer.data(), i);
            REQUIRE(item.next());
            REQUIRE_THROWS_AS(item.get_packed_sint64(), protozero::end_of_buffer_exception);
        }
    }

}

TEST_CASE("write repeated packed sint64 field") {

    std::string buffer;
    protozero::pbf_writer pw(buffer);

    SECTION("empty") {
        int64_t data[] = { 17L };
        pw.add_packed_sint64(1, std::begin(data), std::begin(data) /* !!!! */);

        REQUIRE(buffer == load_data("repeated_packed_sint64/data-empty"));
    }

    SECTION("one") {
        int64_t data[] = { 17L };
        pw.add_packed_sint64(1, std::begin(data), std::end(data));

        REQUIRE(buffer == load_data("repeated_packed_sint64/data-one"));
    }

    SECTION("many") {
        int64_t data[] = { 17L, 0L, 1L, -1L, std::numeric_limits<int64_t>::max(), std::numeric_limits<int64_t>::min() };
        pw.add_packed_sint64(1, std::begin(data), std::end(data));

        REQUIRE(buffer == load_data("repeated_packed_sint64/data-many"));
    }

}

TEST_CASE("write repeated packed sint64 field using packed_field_sint64") {

    std::string buffer;
    protozero::pbf_writer pw(buffer);

    SECTION("empty - should do rollback") {
        {
            protozero::packed_field_sint64 field{pw, 1};
        }

        REQUIRE(buffer == load_data("repeated_packed_sint64/data-empty"));
    }

    SECTION("one") {
        {
            protozero::packed_field_sint64 field{pw, 1};
            field.add_element(17L);
        }

        REQUIRE(buffer == load_data("repeated_packed_sint64/data-one"));
    }

    SECTION("many") {
        {
            protozero::packed_field_sint64 field{pw, 1};
            field.add_element(17L);
            field.add_element( 0L);
            field.add_element( 1L);
            field.add_element(-1L);
            field.add_element(std::numeric_limits<int64_t>::max());
            field.add_element(std::numeric_limits<int64_t>::min());
        }

        REQUIRE(buffer == load_data("repeated_packed_sint64/data-many"));
    }

}

