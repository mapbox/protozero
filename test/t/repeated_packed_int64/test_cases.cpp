
#include <test.hpp>

TEST_CASE("read repeated packed int64 field") {

    SECTION("empty") {
        const std::string buffer = load_data("repeated_packed_int64/data-empty");

        protozero::pbf_reader item(buffer);

        REQUIRE(!item.next());
    }

    SECTION("one") {
        const std::string buffer = load_data("repeated_packed_int64/data-one");

        protozero::pbf_reader item(buffer);

        REQUIRE(item.next());
        auto it_pair = item.get_packed_int64();
        REQUIRE(!item.next());

        REQUIRE(*it_pair.first == 17LL);
        REQUIRE(++it_pair.first == it_pair.second);
    }

    SECTION("many") {
        const std::string buffer = load_data("repeated_packed_int64/data-many");

        protozero::pbf_reader item(buffer);

        REQUIRE(item.next());
        auto it_pair = item.get_packed_int64();
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
        const std::string buffer = load_data("repeated_packed_int64/data-many");

        for (std::string::size_type i = 1; i < buffer.size(); ++i) {
            protozero::pbf_reader item(buffer.data(), i);
            REQUIRE(item.next());
            REQUIRE_THROWS_AS(item.get_packed_int64(), protozero::end_of_buffer_exception);
        }
    }

}

TEST_CASE("write repeated packed int64 field") {

    std::string buffer;
    protozero::pbf_writer pw(buffer);

    SECTION("empty") {
        int64_t data[] = { 17LL };
        pw.add_packed_int64(1, std::begin(data), std::begin(data) /* !!!! */);

        REQUIRE(buffer == load_data("repeated_packed_int64/data-empty"));
    }

    SECTION("one") {
        int64_t data[] = { 17LL };
        pw.add_packed_int64(1, std::begin(data), std::end(data));

        REQUIRE(buffer == load_data("repeated_packed_int64/data-one"));
    }

    SECTION("many") {
        int64_t data[] = { 17LL, 0LL, 1LL, -1LL, std::numeric_limits<int64_t>::max(), std::numeric_limits<int64_t>::min() };
        pw.add_packed_int64(1, std::begin(data), std::end(data));

        REQUIRE(buffer == load_data("repeated_packed_int64/data-many"));
    }

}

TEST_CASE("write repeated packed int64 field using packed_field_int64") {

    std::string buffer;
    protozero::pbf_writer pw(buffer);

    SECTION("empty") {
        {
            protozero::packed_field_int64 field{pw, 1};
        }

        REQUIRE(buffer == load_data("repeated_packed_int64/data-empty"));
    }

    SECTION("one") {
        {
            protozero::packed_field_int64 field{pw, 1};
            field.add_element(17LL);
        }

        REQUIRE(buffer == load_data("repeated_packed_int64/data-one"));
    }

    SECTION("many") {
        {
            protozero::packed_field_int64 field{pw, 1};
            field.add_element(17LL);
            field.add_element( 0LL);
            field.add_element( 1LL);
            field.add_element(-1LL);
            field.add_element(std::numeric_limits<int64_t>::max());
            field.add_element(std::numeric_limits<int64_t>::min());
        }

        REQUIRE(buffer == load_data("repeated_packed_int64/data-many"));
    }

}

