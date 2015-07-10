
#include <test.hpp>

TEST_CASE("exceptions messages") {

    SECTION("pbf") {
        protozero::pbf::exception e;
        REQUIRE(std::string(e.what()) == std::string("pbf exception"));
    }

    SECTION("varint too long") {
        protozero::pbf::varint_too_long_exception e;
        REQUIRE(std::string(e.what()) == std::string("pbf varint too long exception"));
    }

    SECTION("unknown field type") {
        protozero::pbf::unknown_field_type_exception e;
        REQUIRE(std::string(e.what()) == std::string("pbf unknown field type exception"));
    }

    SECTION("end of buffer") {
        protozero::pbf::end_of_buffer_exception e;
        REQUIRE(std::string(e.what()) == std::string("pbf end of buffer exception"));
    }

}

