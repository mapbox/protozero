
#include <test.hpp>

#include "t/bytes/bytes_testcase.pb.h"

TEMPLATE_TEST_CASE("write bytes field and check with libprotobuf", "",
    test_type_string_buffer, test_type_array_buffer) {

    TestType buffer;
    typename TestType::writer_type pw{buffer.buffer()};

    TestBytes::Test msg;

    SECTION("empty") {
        pw.add_string(1, "");

        msg.ParseFromArray(buffer.data(), buffer.size());

        REQUIRE(msg.s().empty());
    }

    SECTION("one") {
        pw.add_string(1, "x");

        msg.ParseFromArray(buffer.data(), buffer.size());

        REQUIRE(msg.s() == "x");
    }

    SECTION("string") {
        pw.add_string(1, "foobar");

        msg.ParseFromArray(buffer.data(), buffer.size());

        REQUIRE(msg.s() == "foobar");
    }

    SECTION("binary") {
        std::string data;
        data.append(1, char(1));
        data.append(1, char(2));
        data.append(1, char(3));

        pw.add_string(1, data);

        msg.ParseFromArray(buffer.data(), buffer.size());

        REQUIRE(msg.s().size() == 3);
        REQUIRE(msg.s()[1] == char(2));
        REQUIRE(msg.s()[2] == char(3));
        REQUIRE(msg.s()[2] == char(3));
    }

}

