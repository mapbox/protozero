
#include <test.hpp>

TEST_CASE("message") {

    SECTION("string") {
        std::string buffer = get_file_data("test/t/message/data-message.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        mapbox::util::pbf subitem { item.message() };
        REQUIRE(!item.next());

        REQUIRE(subitem.next());
        REQUIRE(subitem.string() == "foobar");
        REQUIRE(!subitem.next());
    }

}

