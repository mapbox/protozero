
#include <test.hpp>

TEST_CASE("basic") {

    SECTION("default constructed pbf message is okay") {
        mapbox::util::pbf item;
        REQUIRE(!item.next());
    }

    SECTION("empty buffer is okay") {
        std::string buffer;
        mapbox::util::pbf item(buffer.data(), 0);
        REQUIRE(!item.next());
    }

    SECTION("check every possible value for single byte in buffer") {
        char buffer[1];
        for (int i = 0; i <= 255; ++i) {
            *buffer = static_cast<char>(i);
            mapbox::util::pbf item(buffer, 1);

            REQUIRE_THROWS({
                item.next();
                item.skip();
            });
        }
    }

}

