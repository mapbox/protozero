
#include <test.hpp>

TEST_CASE("read enum field") {

    SECTION("zero") {
        std::string buffer = load_data("enum/data-black");

        protozero::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.get_enum() == 0L);
        REQUIRE(!item.next());
    }

    SECTION("positive") {
        std::string buffer = load_data("enum/data-blue");

        protozero::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.get_enum() == 3L);
        REQUIRE(!item.next());
    }

}

TEST_CASE("write enum field") {

    std::string buffer;
    protozero::pbf_writer pw(buffer);

    SECTION("zero") {
        pw.add_enum(1, 0L);
        REQUIRE(buffer == load_data("enum/data-black"));
    }

    SECTION("positive") {
        pw.add_enum(1, 3L);
        REQUIRE(buffer == load_data("enum/data-blue"));
    }

}

