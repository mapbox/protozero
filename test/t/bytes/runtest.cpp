
#include <test.hpp>

TEST_CASE("bytes") {

    SECTION("empty") {
        std::string buffer = get_file_data("test/t/bytes/data-empty.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.string() == "");
        REQUIRE(!item.next());
    }

    SECTION("one") {
        std::string buffer = get_file_data("test/t/bytes/data-one.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.string() == "x");
        REQUIRE(!item.next());
    }

    SECTION("string") {
        std::string buffer = get_file_data("test/t/bytes/data-string.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.string() == "foobar");
        REQUIRE(!item.next());
    }

    SECTION("string") {
        std::string buffer = get_file_data("test/t/bytes/data-binary.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        std::string data = item.string();
        REQUIRE(data.size() == 3);
        REQUIRE(data[0] == char(1));
        REQUIRE(data[1] == char(2));
        REQUIRE(data[2] == char(3));
        REQUIRE(!item.next());
    }

}

