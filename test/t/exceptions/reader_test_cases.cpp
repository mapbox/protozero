
#include <test.hpp>

TEST_CASE("exceptions messages") {

    SECTION("pbf") {
        protozero::exception e;
        REQUIRE(std::string(e.what()) == std::string("pbf exception"));
    }

    SECTION("varint too long") {
        protozero::varint_too_long_exception e;
        REQUIRE(std::string(e.what()) == std::string("varint too long exception"));
    }

    SECTION("unknown pbf field type") {
        protozero::unknown_pbf_field_type_exception e;
        REQUIRE(std::string(e.what()) == std::string("unknown pbf field type exception"));
    }

    SECTION("end of buffer") {
        protozero::end_of_buffer_exception e;
        REQUIRE(std::string(e.what()) == std::string("end of buffer exception"));
    }

}

