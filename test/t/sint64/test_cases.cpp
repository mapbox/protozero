
#include <test.hpp>

TEST_CASE("read sint64 field") {

    SECTION("zero") {
        const std::string buffer = load_data("sint64/data-zero");

        protozero::pbf_reader item(buffer);

        REQUIRE(item.next());
        REQUIRE(item.get_sint64() == 0LL);
        REQUIRE(!item.next());
    }

    SECTION("positive") {
        const std::string buffer = load_data("sint64/data-pos");

        protozero::pbf_reader item(buffer);

        REQUIRE(item.next());
        REQUIRE(item.get_sint64() == 1LL);
        REQUIRE(!item.next());
    }

    SECTION("negative") {
        const std::string buffer = load_data("sint64/data-neg");

        protozero::pbf_reader item(buffer);

        REQUIRE(item.next());
        REQUIRE(item.get_sint64() == -1LL);
        REQUIRE(!item.next());
    }

    SECTION("max") {
        const std::string buffer = load_data("sint64/data-max");

        protozero::pbf_reader item(buffer);

        REQUIRE(item.next());
        REQUIRE(item.get_sint64() == std::numeric_limits<int64_t>::max());
        REQUIRE(!item.next());
    }

    SECTION("min") {
        const std::string buffer = load_data("sint64/data-min");

        protozero::pbf_reader item(buffer);

        REQUIRE(item.next());
        REQUIRE(item.get_sint64() == std::numeric_limits<int64_t>::min());
        REQUIRE(!item.next());
    }

    SECTION("end_of_buffer") {
        const std::string buffer = load_data("sint64/data-min");

        for (std::string::size_type i = 1; i < buffer.size(); ++i) {
            protozero::pbf_reader item(buffer.data(), i);
            REQUIRE(item.next());
            REQUIRE_THROWS_AS(item.get_sint64(), protozero::end_of_buffer_exception);
        }
    }

}

TEST_CASE("write sint64 field") {

    std::string buffer;
    protozero::pbf_writer pw(buffer);

    SECTION("zero") {
        pw.add_sint64(1, 0L);
        REQUIRE(buffer == load_data("sint64/data-zero"));
    }

    SECTION("positive") {
        pw.add_sint64(1, 1L);
        REQUIRE(buffer == load_data("sint64/data-pos"));
    }

    SECTION("negative") {
        pw.add_sint64(1, -1L);
        REQUIRE(buffer == load_data("sint64/data-neg"));
    }

    SECTION("max") {
        pw.add_sint64(1, std::numeric_limits<int64_t>::max());
        REQUIRE(buffer == load_data("sint64/data-max"));
    }

    SECTION("min") {
        pw.add_sint64(1, std::numeric_limits<int64_t>::min());
        REQUIRE(buffer == load_data("sint64/data-min"));
    }

}

