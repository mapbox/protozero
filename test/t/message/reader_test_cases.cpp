
#include <test.hpp>

TEST_CASE("read message field") {

    SECTION("string") {
        std::string buffer = load_data("message/data-message");

        protozero::pbf_reader item(buffer);

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

    SECTION("optional contents of message - empty") {
        std::string buffer = load_data("message/data-opt-empty");

        protozero::pbf_reader item(buffer);

        REQUIRE(!item.next());
    }

    SECTION("string") {
        std::string buffer = load_data("message/data-opt-element");

        protozero::pbf_reader item(buffer);

        REQUIRE(item.next());
        REQUIRE(item.get_string() == "optional");
        REQUIRE(!item.next());
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

TEST_CASE("write optional message field") {

    std::string buffer_opt;
    protozero::pbf_writer pbf_opt(buffer_opt);

    SECTION("add nothing") {
        REQUIRE(buffer_opt == load_data("message/data-opt-empty"));
    }

    SECTION("add string") {
        pbf_opt.add_string(1, "optional");

        REQUIRE(buffer_opt == load_data("message/data-opt-element"));
    }

}

