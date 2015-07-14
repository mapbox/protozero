
#include <test.hpp>

TEST_CASE("basic") {

    SECTION("default constructed pbf message is okay") {
        protozero::pbf_reader item;

        REQUIRE(item.length() == 0);
        REQUIRE(!item); // test operator bool()
        REQUIRE(!item.next());
    }

    SECTION("empty buffer is okay") {
        std::string buffer;
        protozero::pbf_reader item(buffer.data(), 0);

        REQUIRE(item.length() == 0);
        REQUIRE(!item); // test operator bool()
        REQUIRE(!item.next());
    }

    SECTION("check every possible value for single byte in buffer") {
        char buffer[1];
        for (int i = 0; i <= 255; ++i) {
            *buffer = static_cast<char>(i);
            protozero::pbf_reader item(buffer, 1);

            REQUIRE(item.length() == 1);
            REQUIRE(!!item); // test operator bool()
            REQUIRE_THROWS({
                item.next();
                item.skip();
            });
        }
    }

    SECTION("illegal wire type") {
        char buffer[1] = { 1 << 3 | 7 };

        protozero::pbf_reader item(buffer, 1);
        REQUIRE(item.next());
        REQUIRE_THROWS_AS(item.skip(), protozero::unknown_pbf_wire_type_exception);
    }

}

