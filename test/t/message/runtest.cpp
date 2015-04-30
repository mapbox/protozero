
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

    SECTION("end_of_buffer") {
        std::string buffer = get_file_data("test/t/message/data-message.bin");

        for (size_t i=1; i < buffer.size(); ++i) {
            mapbox::util::pbf item(buffer.data(), i);
            REQUIRE(item.next());
            REQUIRE_THROWS_AS(item.string(), mapbox::util::pbf::end_of_buffer_exception);
        }
    }

}

