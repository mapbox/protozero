
#include <test.hpp>

#include "test/t/string/testcase.pb.h"

TEST_CASE("write string field") {

    std::string buffer;
    mapbox::util::pbf_writer pw(buffer);

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
    mapbox::util::pbf_writer pw(buffer);

    TestString::Test msg;

    SECTION("string") {

        {
            mapbox::util::pbf_subwriter sw(pw, 1);
            sw.append("foo");
            sw.append("bar");
        }

        msg.ParseFromString(buffer);

        REQUIRE(msg.s() == "foobar");
    }

}

