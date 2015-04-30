
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

}

