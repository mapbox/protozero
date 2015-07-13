
#include <test.hpp>

TEST_CASE("read double field") {

    SECTION("zero") {
        std::string buffer = load_data("double/data-zero");

        protozero::pbf_reader item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.get_double() == Approx(0.0));
        REQUIRE(!item.next());
    }

    SECTION("positive") {
        std::string buffer = load_data("double/data-pos");

        protozero::pbf_reader item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.get_double() == Approx(4.893));
        REQUIRE(!item.next());
    }

    SECTION("negative") {
        std::string buffer = load_data("double/data-neg");

        protozero::pbf_reader item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        REQUIRE(item.get_double() == Approx(-9232.33));
        REQUIRE(!item.next());
    }

    SECTION("end_of_buffer") {
        std::string buffer = load_data("double/data-neg");

        for (size_t i=1; i < buffer.size(); ++i) {
            protozero::pbf_reader item(buffer.data(), i);
            REQUIRE(item.next());
            REQUIRE_THROWS_AS(item.get_double(), protozero::end_of_buffer_exception);
        }
    }

}

TEST_CASE("write double field") {

    std::string buffer;
    protozero::pbf_writer pw(buffer);

    SECTION("zero") {
        pw.add_double(1, 0.0);
        REQUIRE(buffer == load_data("double/data-zero"));
    }

    SECTION("positive") {
        pw.add_double(1, 4.893);
        REQUIRE(buffer == load_data("double/data-pos"));
    }

    SECTION("negative") {
        pw.add_double(1, -9232.33);
        REQUIRE(buffer == load_data("double/data-neg"));
    }

}

