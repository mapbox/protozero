
#include <test.hpp>

TEST_CASE("basic") {

    SECTION("default constructed pbf message is okay") {
        mapbox::util::pbf item;

        REQUIRE(!item); // test operator bool()
        REQUIRE(!item.next());
    }

    SECTION("empty buffer is okay") {
        std::string buffer;
        mapbox::util::pbf item(buffer.data(), 0);

        REQUIRE(!item); // test operator bool()
        REQUIRE(!item.next());
    }

    SECTION("check every possible value for single byte in buffer") {
        char buffer[1];
        for (int i = 0; i <= 255; ++i) {
            *buffer = static_cast<char>(i);
            mapbox::util::pbf item(buffer, 1);

            REQUIRE(!!item); // test operator bool()
            REQUIRE_THROWS({
                item.next();
                item.skip();
            });
        }
    }

    SECTION("illegal wire type") {
        char buffer[1] = { 1 << 3 | 7 };

        mapbox::util::pbf item(buffer, 1);
        REQUIRE(item.next());
        REQUIRE_THROWS_AS(item.skip(), mapbox::util::pbf::unknown_field_type_exception);
    }

}

