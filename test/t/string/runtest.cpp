
#include <test.hpp>

TEST_CASE("read string field") {

    SECTION("empty") {
        std::string buffer = load_data("string/data-empty");

        protozero::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.get_string() == "");
        REQUIRE(!item.next());
    }

    SECTION("one") {
        std::string buffer = load_data("string/data-one");

        protozero::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.get_string() == "x");
        REQUIRE(!item.next());
    }

    SECTION("string") {
        std::string buffer = load_data("string/data-string");

        protozero::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.get_string() == "foobar");
        REQUIRE(!item.next());
    }

    SECTION("end_of_buffer") {
        std::string buffer = load_data("string/data-string");

        for (size_t i=1; i < buffer.size(); ++i) {
            protozero::pbf item(buffer.data(), i);
            REQUIRE(item.next());
            REQUIRE_THROWS_AS(item.get_string(), protozero::pbf::end_of_buffer_exception);
        }
    }

    SECTION("check assert on fixed int access") {
        std::string buffer = load_data("string/data-string");

        protozero::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE_THROWS_AS(item.get_fixed32(), assert_error);
    }

}

TEST_CASE("write string field") {

    std::string buffer;
    protozero::pbf_writer pw(buffer);

    SECTION("empty") {
        pw.add_string(1, "");
        REQUIRE(buffer == load_data("string/data-empty"));
    }

    SECTION("one") {
        pw.add_string(1, "x");
        REQUIRE(buffer == load_data("string/data-one"));
    }

    SECTION("string") {
        pw.add_string(1, "foobar");
        REQUIRE(buffer == load_data("string/data-string"));
    }

}

TEST_CASE("write string field with subwriter") {

    std::string buffer;
    protozero::pbf_writer pw(buffer);

    SECTION("string") {

        {
            protozero::pbf_subwriter sw(pw, 1);
            sw.append("foo");
            sw.append("bar");
        }

        REQUIRE(buffer == load_data("string/data-string"));
    }

}

