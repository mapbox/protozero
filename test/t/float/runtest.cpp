
#include <test.hpp>

TEST_CASE("float") {

    SECTION("zero") {
        std::string buffer = get_file_data("test/t/float/data-zero.pbf");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.get_float() == Approx(0.0f));
        REQUIRE(!item.next());
    }

    SECTION("positive") {
        std::string buffer = get_file_data("test/t/float/data-pos.pbf");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.get_float() == Approx(5.34f));
        REQUIRE(!item.next());
    }

    SECTION("negative") {
        std::string buffer = get_file_data("test/t/float/data-neg.pbf");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.get_float() == Approx(-1.71f));
        REQUIRE(!item.next());
    }

    SECTION("end_of_buffer") {
        std::string buffer = get_file_data("test/t/float/data-neg.pbf");

        for (size_t i=1; i < buffer.size(); ++i) {
            mapbox::util::pbf item(buffer.data(), i);
            REQUIRE(item.next());
            REQUIRE_THROWS_AS(item.get_float(), mapbox::util::pbf::end_of_buffer_exception);
        }
    }

}

TEST_CASE("write float") {

    SECTION("zero") {
        std::string buffer = get_file_data("test/t/float/data-zero.pbf");

        std::string wbuffer;
        mapbox::util::pbf_writer pw(wbuffer);
        pw.add_float(1, 0.0f);

        REQUIRE(buffer == wbuffer);
    }

    SECTION("positive") {
        std::string buffer = get_file_data("test/t/float/data-pos.pbf");

        std::string wbuffer;
        mapbox::util::pbf_writer pw(wbuffer);
        pw.add_float(1, 5.34f);

        REQUIRE(buffer == wbuffer);
    }

    SECTION("negative") {
        std::string buffer = get_file_data("test/t/float/data-neg.pbf");

        std::string wbuffer;
        mapbox::util::pbf_writer pw(wbuffer);
        pw.add_float(1, -1.71f);

        REQUIRE(buffer == wbuffer);
    }

}

