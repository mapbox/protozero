
#include <string>

#include <buffer.hpp>

#include "t/bool/bool_testcase.pb.h"

TEMPLATE_TEST_CASE("write bool field and check with libprotobuf", "",
    buffer_test_string, buffer_test_vector, buffer_test_array, buffer_test_external) {

    TestType buffer;
    typename TestType::writer_type pw{buffer.buffer()};

    TestBoolean::Test msg;

    SECTION("false") {
        pw.add_bool(1, false);

        REQUIRE(msg.ParseFromArray(buffer.data(), buffer.size()));

        REQUIRE_FALSE(msg.b());
    }

    SECTION("true") {
        pw.add_bool(1, true);

        REQUIRE(msg.ParseFromArray(buffer.data(), buffer.size()));

        REQUIRE(msg.b());
    }

}

