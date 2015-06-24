
#include <test.hpp>

#include "test/t/message/testcase.pb.h"

TEST_CASE("write message field") {

    std::string buffer;
    mapbox::util::pbf_writer pw(buffer);

    TestMessage::Test msg;

    SECTION("string") {
        std::string sbuffer;
        mapbox::util::pbf_writer pws(sbuffer);
        pws.add_string(1, "foobar");

        pw.add_message(1, sbuffer);
    }

    SECTION("string with subwriter") {
        mapbox::util::pbf_subwriter sw(pw, 1);
        pw.add_string(1, "foobar");

    }

    msg.ParseFromString(buffer);
    REQUIRE(msg.submessage().s() == "foobar");

}

