
#include <test.hpp>

#include "test/t/nested/testcase.pb.h"

TEST_CASE("write nested message fields") {

    std::string buffer;
    protozero::pbf_writer pw(buffer);

    TestNested::Test msg;

    SECTION("string") {
        std::string ssbuffer;
        protozero::pbf_writer pwss(ssbuffer);
        pwss.add_string(1, "foobar");
        pwss.add_int32(2, 99);

        std::string sbuffer;
        protozero::pbf_writer pws(sbuffer);
        pws.add_string(1, ssbuffer);
        pws.add_int32(2, 88);

        pw.add_message(1, sbuffer);
    }

    SECTION("string with subwriter") {
        protozero::pbf_subwriter sw(pw, 1);
        {
            protozero::pbf_subwriter ssw(pw, 1);
            pw.add_string(1, "foobar");
            pw.add_int32(2, 99);
        }
        pw.add_int32(2, 88);
    }

    pw.add_int32(2, 77);

    msg.ParseFromString(buffer);

    REQUIRE(msg.i() == 77);
    REQUIRE(msg.sub().i() == 88);
    REQUIRE(msg.sub().subsub().i() == 99);
    REQUIRE(msg.sub().subsub().s() == "foobar");
}

