
#include <test.hpp>

TEST_CASE("bool") {

    SECTION("false") {
        std::string buffer = get_file_data("test/t/bool/data-false.pbf");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(!item.get_bool());
        REQUIRE(!item.next());
    }

    SECTION("true") {
        std::string buffer = get_file_data("test/t/bool/data-true.pbf");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.get_bool());
        REQUIRE(!item.next());
    }

    SECTION("also true") {
        std::string buffer = get_file_data("test/t/bool/data-also-true.pbf");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.get_bool());
        REQUIRE(!item.next());
    }

    SECTION("still true") {
        std::string buffer = get_file_data("test/t/bool/data-still-true.pbf");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.get_bool());
        REQUIRE(!item.next());
    }

}

TEST_CASE("write bool") {

    SECTION("false") {
        std::string buffer = get_file_data("test/t/bool/data-false.pbf");

        std::string wbuffer;
        mapbox::util::pbf_writer pw(wbuffer);
        pw.add_bool(1, false);

        REQUIRE(buffer == wbuffer);
    }

    SECTION("true") {
        std::string buffer = get_file_data("test/t/bool/data-true.pbf");

        std::string wbuffer;
        mapbox::util::pbf_writer pw(wbuffer);
        pw.add_bool(1, true);

        REQUIRE(buffer == wbuffer);
    }

}

