
#include <string>

#include <test.hpp> // IWYU pragma: keep

#include "t/bool/bool_testcase.pb.h"

TEMPLATE_TEST_CASE("write bool field and check with libprotobuf", "",
    test_type_string_buffer, test_type_vector_buffer, test_type_array_buffer) {

    TestType buffer;
    typename TestType::writer_type pw{buffer.buffer()};

    TestBoolean::Test msg;

    SECTION("false") {
        pw.add_bool(1, false);

        msg.ParseFromArray(buffer.data(), buffer.size());

        REQUIRE_FALSE(msg.b());
    }

    SECTION("true") {
        pw.add_bool(1, true);

        msg.ParseFromArray(buffer.data(), buffer.size());

        REQUIRE(msg.b());
    }

}

