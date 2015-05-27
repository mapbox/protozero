
#include <test.hpp>

TEST_CASE("exceptions messages") {

    SECTION("pbf") {
        mapbox::util::pbf::exception e;
        REQUIRE(e.what() == "pbf exception");
    }

    SECTION("varint too long") {
        mapbox::util::pbf::varint_too_long_exception e;
        REQUIRE(e.what() == "pbf varint too long exception");
    }

    SECTION("unknown field type") {
        mapbox::util::pbf::unknown_field_type_exception e;
        REQUIRE(e.what() == "pbf unknown field type exception");
    }

    SECTION("end of buffer") {
        mapbox::util::pbf::end_of_buffer_exception e;
        REQUIRE(e.what() == "pbf end of buffer exception");
    }

}

