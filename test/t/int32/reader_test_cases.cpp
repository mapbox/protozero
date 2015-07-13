
#include <test.hpp>

TEST_CASE("read int32 field") {

    SECTION("zero") {
        std::string buffer = load_data("int32/data-zero");

        protozero::pbf_reader item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.get_int32() == 0L);
        REQUIRE(!item.next());
    }

    SECTION("positive") {
        std::string buffer = load_data("int32/data-pos");

        protozero::pbf_reader item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.get_int32() == 1L);
        REQUIRE(!item.next());
    }

    SECTION("negative") {
        std::string buffer = load_data("int32/data-neg");

        protozero::pbf_reader item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.get_int32() == -1L);
        REQUIRE(!item.next());
    }

    SECTION("max") {
        std::string buffer = load_data("int32/data-max");

        protozero::pbf_reader item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.get_int32() == std::numeric_limits<int32_t>::max());
        REQUIRE(!item.next());
    }

    SECTION("min64") {
        std::string buffer = load_data("int32/data-min");

        protozero::pbf_reader item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(static_cast<int32_t>(item.get_int64()) == std::numeric_limits<int32_t>::min());
        REQUIRE(!item.next());
    }

    SECTION("min") {
        std::string buffer = load_data("int32/data-min");

        protozero::pbf_reader item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.get_int32() == std::numeric_limits<int32_t>::min());
        REQUIRE(!item.next());
    }

    SECTION("end_of_buffer") {
        std::string buffer = load_data("int32/data-min");

        for (size_t i=1; i < buffer.size(); ++i) {
            protozero::pbf_reader item(buffer.data(), i);
            REQUIRE(item.next());
            REQUIRE_THROWS_AS(item.get_int32(), protozero::end_of_buffer_exception);
        }
    }

    SECTION("check assert on string/fixed int access") {
        std::string buffer = load_data("int32/data-zero");

        protozero::pbf_reader item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE_THROWS_AS(item.get_fixed32(), assert_error);
        REQUIRE_THROWS_AS(item.get_string(), assert_error);
    }

}

TEST_CASE("write int32 field") {

    std::string buffer;
    protozero::pbf_writer pw(buffer);

    SECTION("zero") {
        pw.add_int32(1, 0L);
        REQUIRE(buffer == load_data("int32/data-zero"));
    }

    SECTION("positive") {
        pw.add_int32(1, 1L);
        REQUIRE(buffer == load_data("int32/data-pos"));
    }

    SECTION("negative") {
        pw.add_int32(1, -1L);
        REQUIRE(buffer == load_data("int32/data-neg"));
    }

    SECTION("max") {
        pw.add_int32(1, std::numeric_limits<int32_t>::max());
        REQUIRE(buffer == load_data("int32/data-max"));
    }

    SECTION("min") {
        pw.add_int32(1, std::numeric_limits<int32_t>::min());
        REQUIRE(buffer == load_data("int32/data-min"));
    }

}

