
#include <test.hpp>

TEST_CASE("boolean") {

    SECTION("false") {
        std::string buffer = get_file_data("test/t/boolean/data-false.pbf");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(!item.get_bool());
        REQUIRE(!item.next());
    }

    SECTION("true") {
        std::string buffer = get_file_data("test/t/boolean/data-true.pbf");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.get_bool());
        REQUIRE(!item.next());
    }

    SECTION("also true") {
        std::string buffer = get_file_data("test/t/boolean/data-also-true.pbf");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.get_bool());
        REQUIRE(!item.next());
    }

    SECTION("still true") {
        std::string buffer = get_file_data("test/t/boolean/data-still-true.pbf");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.get_bool());
        REQUIRE(!item.next());
    }

}

TEST_CASE("write boolean") {

    SECTION("false") {
        std::string buffer = get_file_data("test/t/boolean/data-false.pbf");

        std::string wbuffer;
        mapbox::util::pbf_writer pw(wbuffer);
        pw.add_bool(1, false);

        REQUIRE(buffer == wbuffer);
    }

    SECTION("true") {
        std::string buffer = get_file_data("test/t/boolean/data-true.pbf");

        std::string wbuffer;
        mapbox::util::pbf_writer pw(wbuffer);
        pw.add_bool(1, true);

        REQUIRE(buffer == wbuffer);
    }

}

