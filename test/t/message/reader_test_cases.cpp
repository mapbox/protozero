
#include <test.hpp>

TEST_CASE("read message field") {

    SECTION("string") {
        std::string buffer = load_data("message/data-message");

        protozero::pbf_reader item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        protozero::pbf_reader subitem { item.get_message() };
        REQUIRE(!item.next());

        REQUIRE(subitem.next());
        REQUIRE(subitem.get_string() == "foobar");
        REQUIRE(!subitem.next());
    }

    SECTION("end_of_buffer") {
        std::string buffer = load_data("message/data-message");

        for (size_t i=1; i < buffer.size(); ++i) {
            protozero::pbf_reader item(buffer.data(), i);
            REQUIRE(item.next());
            REQUIRE_THROWS_AS(item.get_string(), protozero::end_of_buffer_exception);
        }
    }

}

TEST_CASE("write message field") {

    std::string buffer_test;
    protozero::pbf_writer pbf_test(buffer_test);

    SECTION("string") {
        std::string buffer_submessage;
        protozero::pbf_writer pbf_submessage(buffer_submessage);
        pbf_submessage.add_string(1, "foobar");

        pbf_test.add_message(1, buffer_submessage);
    }

    SECTION("string with subwriter") {
        protozero::pbf_writer pbf_submessage(pbf_test, 1);
        pbf_submessage.add_string(1, "foobar");
    }

    REQUIRE(buffer_test == load_data("message/data-message"));

}

