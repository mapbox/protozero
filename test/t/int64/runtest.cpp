
#include <test.hpp>

TEST_CASE("read int64 field") {

    SECTION("zero") {
        std::string buffer = load_data("int64/data-zero");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.get_int64() == 0LL);
        REQUIRE(!item.next());
    }

    SECTION("positive") {
        std::string buffer = load_data("int64/data-pos");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.get_int64() == 1LL);
        REQUIRE(!item.next());
    }

    SECTION("negative") {
        std::string buffer = load_data("int64/data-neg");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.get_int64() == -1LL);
        REQUIRE(!item.next());
    }

    SECTION("max") {
        std::string buffer = load_data("int64/data-max");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.get_int64() == std::numeric_limits<int64_t>::max());
        REQUIRE(!item.next());
    }

    SECTION("min") {
        std::string buffer = load_data("int64/data-min");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.get_int64() == std::numeric_limits<int64_t>::min());
        REQUIRE(!item.next());
    }

    SECTION("end_of_buffer") {
        std::string buffer = load_data("int64/data-min");

        for (size_t i=1; i < buffer.size(); ++i) {
            mapbox::util::pbf item(buffer.data(), i);
            REQUIRE(item.next());
            REQUIRE_THROWS_AS(item.get_int64(), mapbox::util::pbf::end_of_buffer_exception);
        }
    }

    SECTION("varint overflow") {
        std::string buffer = load_data("int64/data-overflow");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE_THROWS_AS(item.get_int64(), mapbox::util::pbf::varint_too_long_exception);
    }

}

TEST_CASE("write int64 field") {

    std::string buffer;
    mapbox::util::pbf_writer pw(buffer);

    SECTION("zero") {
        pw.add_int64(1, 0LL);
        REQUIRE(buffer == load_data("int64/data-zero"));
    }

    SECTION("positive") {
        pw.add_int64(1, 1LL);
        REQUIRE(buffer == load_data("int64/data-pos"));
    }

    SECTION("negative") {
        pw.add_int64(1, -1LL);
        REQUIRE(buffer == load_data("int64/data-neg"));
    }

    SECTION("max") {
        pw.add_int64(1, std::numeric_limits<int64_t>::max());
        REQUIRE(buffer == load_data("int64/data-max"));
    }

    SECTION("min") {
        pw.add_int64(1, std::numeric_limits<int64_t>::min());
        REQUIRE(buffer == load_data("int64/data-min"));
    }

}

