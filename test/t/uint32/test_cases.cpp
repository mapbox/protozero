
#include <test.hpp>

TEST_CASE("read uint32 field") {

    SECTION("zero") {
        const std::string buffer = load_data("uint32/data-zero");

        protozero::pbf_reader item(buffer);

        REQUIRE(item.next());
        REQUIRE(item.get_uint32() == 0ul);
        REQUIRE(!item.next());
    }

    SECTION("positive") {
        const std::string buffer = load_data("uint32/data-pos");

        protozero::pbf_reader item(buffer);

        REQUIRE(item.next());
        REQUIRE(item.get_uint32() == 1ul);
        REQUIRE(!item.next());
    }

    SECTION("max") {
        const std::string buffer = load_data("uint32/data-max");

        protozero::pbf_reader item(buffer);

        REQUIRE(item.next());
        REQUIRE(item.get_uint32() == std::numeric_limits<uint32_t>::max());
        REQUIRE(!item.next());
    }

    SECTION("end_of_buffer") {
        const std::string buffer = load_data("uint32/data-max");

        for (size_t i=1; i < buffer.size(); ++i) {
            protozero::pbf_reader item(buffer.data(), i);
            REQUIRE(item.next());
            REQUIRE_THROWS_AS(item.get_uint32(), protozero::end_of_buffer_exception);
        }
    }

}

TEST_CASE("write uint32 field") {

    std::string buffer;
    protozero::pbf_writer pw(buffer);

    SECTION("zero") {
        pw.add_uint32(1, 0UL);
        REQUIRE(buffer == load_data("uint32/data-zero"));
    }

    SECTION("positive") {
        pw.add_uint32(1, 1UL);
        REQUIRE(buffer == load_data("uint32/data-pos"));
    }

    SECTION("max") {
        pw.add_uint32(1, std::numeric_limits<uint32_t>::max());
        REQUIRE(buffer == load_data("uint32/data-max"));
    }

}

