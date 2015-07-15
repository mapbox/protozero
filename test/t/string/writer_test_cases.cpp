
#include <test.hpp>

#include "test/t/string/testcase.pb.h"

TEST_CASE("write string field") {

    std::string buffer;
    protozero::pbf_writer pw(buffer);

    TestString::Test msg;

    SECTION("empty") {
        pw.add_string(1, "");

        msg.ParseFromString(buffer);

        REQUIRE(msg.s() == "");
    }

    SECTION("one") {
        pw.add_string(1, "x");

        msg.ParseFromString(buffer);

        REQUIRE(msg.s() == "x");
    }

    SECTION("string") {
        pw.add_string(1, "foobar");

        msg.ParseFromString(buffer);

        REQUIRE(msg.s() == "foobar");
    }

}

TEST_CASE("write string field with subwriter") {

    std::string buffer;
    protozero::pbf_writer pw(buffer);

    TestString::Test msg;

    SECTION("string") {

        {
            protozero::pbf_writer sw(pw, 1);
            sw.append_sub("foo");
            sw.append_sub("bar");
        }

        msg.ParseFromString(buffer);

        REQUIRE(msg.s() == "foobar");
    }

}

