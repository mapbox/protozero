
#include <test.hpp>

#include "test/t/enum/testcase.pb.h"

TEST_CASE("write enum field") {

    std::string buffer;
    protozero::pbf_writer pw(buffer);

    TestEnum::Test msg;

    SECTION("zero") {
        pw.add_enum(1, 0L);

        msg.ParseFromString(buffer);

        REQUIRE(msg.color() == TestEnum::Color::BLACK);
    }

    SECTION("positive") {
        pw.add_enum(1, 3L);

        msg.ParseFromString(buffer);

        REQUIRE(msg.color() == TestEnum::Color::BLUE);
    }

}

