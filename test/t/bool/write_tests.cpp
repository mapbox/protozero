
#include <string>

#include <test.hpp> // IWYU pragma: keep

#include "test/t/bool/testcase.pb.h"

TEST_CASE("write bool field") {

    std::string buffer;
    mapbox::util::pbf_writer pw(buffer);

    TestBoolean::Test msg;

    SECTION("false") {
        pw.add_bool(1, false);

        msg.ParseFromString(buffer);

        REQUIRE(!msg.b());
    }

    SECTION("true") {
        pw.add_bool(1, true);

        msg.ParseFromString(buffer);

        REQUIRE(msg.b());
    }

}

