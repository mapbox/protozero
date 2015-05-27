
#include <test.hpp>

TEST_CASE("read message field") {

    SECTION("string") {
        std::string buffer = load_data("message/data-message");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        mapbox::util::pbf subitem { item.get_message() };
        REQUIRE(!item.next());

        REQUIRE(subitem.next());
        REQUIRE(subitem.get_string() == "foobar");
        REQUIRE(!subitem.next());
    }

    SECTION("end_of_buffer") {
        std::string buffer = load_data("message/data-message");

        for (size_t i=1; i < buffer.size(); ++i) {
            mapbox::util::pbf item(buffer.data(), i);
            REQUIRE(item.next());
            REQUIRE_THROWS_AS(item.get_string(), mapbox::util::pbf::end_of_buffer_exception);
        }
    }

}

TEST_CASE("write message field") {

    std::string buffer;
    mapbox::util::pbf_writer pw(buffer);

    SECTION("string") {
        std::string sbuffer;
        mapbox::util::pbf_writer pws(sbuffer);
        pws.add_string(1, "foobar");

        pw.add_message(1, sbuffer);
    }

    SECTION("string with subwriter") {
        mapbox::util::pbf_subwriter sw(pw, 1);
        pw.add_string(1, "foobar");
    }

    REQUIRE(buffer == load_data("message/data-message"));

}

