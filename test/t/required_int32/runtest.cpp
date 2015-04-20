
#include <test.hpp>

TEST_CASE("required_int32") {

    SECTION("positive") {
        std::string buffer = get_file_data("test/t/required_int32/data1.bin");

        protobuf::message item(buffer.data(), buffer.size());

        item.next();
            REQUIRE(item.varint() == 17);
        while (item.next()) {
            REQUIRE(item.varint() == 17);
        }
    }

    SECTION("negativ") {
        std::string buffer = get_file_data("test/t/required_int32/data2.bin");

        protobuf::message item(buffer.data(), buffer.size());

        while (item.next()) {
            REQUIRE(item.varint() == -1);
        }
    }

}

