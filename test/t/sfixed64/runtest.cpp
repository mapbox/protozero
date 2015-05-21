
#include <test.hpp>

TEST_CASE("sfixed64") {

    SECTION("zero") {
        std::string buffer = get_file_data("test/t/sfixed64/data-zero.pbf");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.get_sfixed64() == 0LL);
        REQUIRE(!item.next());
    }

    SECTION("max-int") {
        std::string buffer = get_file_data("test/t/sfixed64/data-max-int.pbf");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.get_sfixed64() == std::numeric_limits<int64_t>::max());
        REQUIRE(!item.next());
    }

    SECTION("min-int") {
        std::string buffer = get_file_data("test/t/sfixed64/data-min-int.pbf");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.get_sfixed64() == std::numeric_limits<int64_t>::min());
        REQUIRE(!item.next());
    }

    SECTION("end_of_buffer") {
        std::string buffer = get_file_data("test/t/sfixed64/data-min-int.pbf");

        for (size_t i=1; i < buffer.size(); ++i) {
            mapbox::util::pbf item(buffer.data(), i);
            REQUIRE(item.next());
            REQUIRE_THROWS_AS(item.get_sfixed64(), mapbox::util::pbf::end_of_buffer_exception);
        }
    }

}

TEST_CASE("write sfixed64") {

    SECTION("zero") {
        std::string buffer = get_file_data("test/t/sfixed64/data-zero.pbf");

        std::string wbuffer;
        mapbox::util::pbf_writer pw(wbuffer);
        pw.add_sfixed64(1, 0);

        REQUIRE(buffer == wbuffer);
    }

    SECTION("max-uint") {
        std::string buffer = get_file_data("test/t/sfixed64/data-max-int.pbf");

        std::string wbuffer;
        mapbox::util::pbf_writer pw(wbuffer);
        pw.add_sfixed64(1, std::numeric_limits<int64_t>::max());

        REQUIRE(buffer == wbuffer);
    }

    SECTION("min-uint") {
        std::string buffer = get_file_data("test/t/sfixed64/data-min-int.pbf");

        std::string wbuffer;
        mapbox::util::pbf_writer pw(wbuffer);
        pw.add_sfixed64(1, std::numeric_limits<int64_t>::min());

        REQUIRE(buffer == wbuffer);
    }

}

