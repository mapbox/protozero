
#include <test.hpp>

TEST_CASE("message") {

    SECTION("string") {
        std::string buffer = get_file_data("test/t/message/data-message.pbf");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        mapbox::util::pbf subitem { item.get_message() };
        REQUIRE(!item.next());

        REQUIRE(subitem.next());
        REQUIRE(subitem.get_string() == "foobar");
        REQUIRE(!subitem.next());
    }

    SECTION("end_of_buffer") {
        std::string buffer = get_file_data("test/t/message/data-message.pbf");

        for (size_t i=1; i < buffer.size(); ++i) {
            mapbox::util::pbf item(buffer.data(), i);
            REQUIRE(item.next());
            REQUIRE_THROWS_AS(item.get_string(), mapbox::util::pbf::end_of_buffer_exception);
        }
    }

}

TEST_CASE("write message") {

    SECTION("string") {
        std::string buffer = get_file_data("test/t/message/data-message.pbf");

        std::string sbuffer;
        mapbox::util::pbf_writer pws(sbuffer);
        pws.add_string(1, "foobar");

        std::string wbuffer;
        mapbox::util::pbf_writer pw(wbuffer);
        pw.add_message(1, sbuffer);

        REQUIRE(buffer == wbuffer);
    }
}

TEST_CASE("write message with subwriter") {

    SECTION("string") {
        std::string buffer = get_file_data("test/t/message/data-message.pbf");

        std::string wbuffer;
        mapbox::util::pbf_writer pw(wbuffer);

        {
            mapbox::util::pbf_subwriter sw(pw, 1);
            pw.add_string(1, "foobar");
        }

        REQUIRE(buffer == wbuffer);
    }
}

