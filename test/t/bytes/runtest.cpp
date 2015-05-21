
#include <test.hpp>

TEST_CASE("bytes") {

    SECTION("empty") {
        std::string buffer = get_file_data("test/t/bytes/data-empty.pbf");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.get_string() == "");
        REQUIRE(!item.next());
    }

    SECTION("one") {
        std::string buffer = get_file_data("test/t/bytes/data-one.pbf");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.get_string() == "x");
        REQUIRE(!item.next());
    }

    SECTION("string") {
        std::string buffer = get_file_data("test/t/bytes/data-string.pbf");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.get_string() == "foobar");
        REQUIRE(!item.next());
    }

    SECTION("binary") {
        std::string buffer = get_file_data("test/t/bytes/data-binary.pbf");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        std::string data = item.get_string();
        REQUIRE(data.size() == 3);
        REQUIRE(data[0] == char(1));
        REQUIRE(data[1] == char(2));
        REQUIRE(data[2] == char(3));
        REQUIRE(!item.next());
    }

    SECTION("end_of_buffer") {
        std::string buffer = get_file_data("test/t/bytes/data-binary.pbf");

        for (size_t i=1; i < buffer.size(); ++i) {
            mapbox::util::pbf item(buffer.data(), i);
            REQUIRE(item.next());
            REQUIRE_THROWS_AS(item.get_string(), mapbox::util::pbf::end_of_buffer_exception);
        }
    }

}

TEST_CASE("write bytes") {

    SECTION("empty") {
        std::string buffer = get_file_data("test/t/bytes/data-empty.pbf");

        std::string wbuffer;
        mapbox::util::pbf_writer pw(wbuffer);
        pw.add_string(1, "");

        REQUIRE(buffer == wbuffer);
    }

    SECTION("one") {
        std::string buffer = get_file_data("test/t/bytes/data-one.pbf");

        std::string wbuffer;
        mapbox::util::pbf_writer pw(wbuffer);
        pw.add_string(1, "x");

        REQUIRE(buffer == wbuffer);
    }

    SECTION("string") {
        std::string buffer = get_file_data("test/t/bytes/data-string.pbf");

        std::string wbuffer;
        mapbox::util::pbf_writer pw(wbuffer);
        pw.add_string(1, "foobar");

        REQUIRE(buffer == wbuffer);
    }

    SECTION("binary") {
        std::string buffer = get_file_data("test/t/bytes/data-binary.pbf");

        std::string data;
        data.append(1, char(1));
        data.append(1, char(2));
        data.append(1, char(3));

        std::string wbuffer;
        mapbox::util::pbf_writer pw(wbuffer);
        pw.add_string(1, data);

        REQUIRE(buffer == wbuffer);
    }

}

