
#include <test.hpp>

TEST_CASE("read bool field") {

    SECTION("false") {
        std::string buffer = load_data("bool/data-false");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(!item.get_bool());
        REQUIRE(!item.next());
    }

    SECTION("true") {
        std::string buffer = load_data("bool/data-true");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.get_bool());
        REQUIRE(!item.next());
    }

    SECTION("also true") {
        std::string buffer = load_data("bool/data-also-true");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.get_bool());
        REQUIRE(!item.next());
    }

    SECTION("still true") {
        std::string buffer = load_data("bool/data-still-true");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.get_bool());
        REQUIRE(!item.next());
    }

}

TEST_CASE("write bool field") {

    std::string buffer;
    mapbox::util::pbf_writer pw(buffer);

    SECTION("false") {
        pw.add_bool(1, false);
        REQUIRE(buffer == load_data("bool/data-false"));
    }

    SECTION("true") {
        pw.add_bool(1, true);
        REQUIRE(buffer == load_data("bool/data-true"));
    }

}

