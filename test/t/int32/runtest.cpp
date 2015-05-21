
#include <test.hpp>

TEST_CASE("int32") {

    SECTION("zero") {
        std::string buffer = get_file_data("test/t/int32/data-zero.pbf");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.get_int32() == 0L);
        REQUIRE(!item.next());
    }

    SECTION("positive") {
        std::string buffer = get_file_data("test/t/int32/data-pos.pbf");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.get_int32() == 1L);
        REQUIRE(!item.next());
    }

    SECTION("negative") {
        std::string buffer = get_file_data("test/t/int32/data-neg.pbf");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.get_int32() == -1L);
        REQUIRE(!item.next());
    }

    SECTION("max") {
        std::string buffer = get_file_data("test/t/int32/data-max.pbf");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.get_int32() == std::numeric_limits<int32_t>::max());
        REQUIRE(!item.next());
    }

    SECTION("min64") {
        std::string buffer = get_file_data("test/t/int32/data-min.pbf");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(static_cast<int32_t>(item.get_int64()) == std::numeric_limits<int32_t>::min());
        REQUIRE(!item.next());
    }

    SECTION("min") {
        std::string buffer = get_file_data("test/t/int32/data-min.pbf");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.get_int32() == std::numeric_limits<int32_t>::min());
        REQUIRE(!item.next());
    }

    SECTION("end_of_buffer") {
        std::string buffer = get_file_data("test/t/int32/data-min.pbf");

        for (size_t i=1; i < buffer.size(); ++i) {
            mapbox::util::pbf item(buffer.data(), i);
            REQUIRE(item.next());
            REQUIRE_THROWS_AS(item.get_int32(), mapbox::util::pbf::end_of_buffer_exception);
        }
    }

    SECTION("check assert on string/fixed int access") {
        std::string buffer = get_file_data("test/t/int32/data-zero.pbf");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE_THROWS_AS(item.get_fixed32(), assert_error);
        REQUIRE_THROWS_AS(item.get_string(), assert_error);
    }

}

TEST_CASE("write int32") {

    SECTION("zero") {
        std::string buffer = get_file_data("test/t/int32/data-zero.pbf");

        std::string wbuffer;
        mapbox::util::pbf_writer pw(wbuffer);
        pw.add_int32(1, 0L);

        REQUIRE(buffer == wbuffer);
    }

    SECTION("positive") {
        std::string buffer = get_file_data("test/t/int32/data-pos.pbf");

        std::string wbuffer;
        mapbox::util::pbf_writer pw(wbuffer);
        pw.add_int32(1, 1L);

        REQUIRE(buffer == wbuffer);
    }

    SECTION("negative") {
        std::string buffer = get_file_data("test/t/int32/data-neg.pbf");

        std::string wbuffer;
        mapbox::util::pbf_writer pw(wbuffer);
        pw.add_int32(1, -1L);

        REQUIRE(buffer == wbuffer);
    }

    SECTION("max") {
        std::string buffer = get_file_data("test/t/int32/data-max.pbf");

        std::string wbuffer;
        mapbox::util::pbf_writer pw(wbuffer);
        pw.add_int32(1, std::numeric_limits<int32_t>::max());

        REQUIRE(buffer == wbuffer);
    }

    SECTION("min") {
        std::string buffer = get_file_data("test/t/int32/data-min.pbf");

        std::string wbuffer;
        mapbox::util::pbf_writer pw(wbuffer);
        pw.add_int32(1, std::numeric_limits<int32_t>::min());

        REQUIRE(buffer == wbuffer);
    }

}

