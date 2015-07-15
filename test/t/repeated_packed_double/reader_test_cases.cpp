
#include <test.hpp>

TEST_CASE("read repeated packed double field") {

    SECTION("empty") {
        std::string buffer = load_data("repeated_packed_double/data-empty");

        protozero::pbf_reader item(buffer.data(), buffer.size());

        REQUIRE(!item.next());
    }

    SECTION("one") {
        std::string buffer = load_data("repeated_packed_double/data-one");

        protozero::pbf_reader item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        auto it_pair = item.get_packed_double();
        REQUIRE(!item.next());

        REQUIRE(*it_pair.first == 17.34);
        REQUIRE(++it_pair.first == it_pair.second);
    }

    SECTION("many") {
        std::string buffer = load_data("repeated_packed_double/data-many");

        protozero::pbf_reader item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        auto it_pair = item.get_packed_double();
        REQUIRE(!item.next());

        auto it = it_pair.first;
        REQUIRE(*it++ == 17.34);
        REQUIRE(*it++ ==   0.0);
        REQUIRE(*it++ ==   1.0);
        REQUIRE(*it++ == std::numeric_limits<double>::min());
        REQUIRE(*it++ == std::numeric_limits<double>::max());
        REQUIRE(it == it_pair.second);
    }

    SECTION("end_of_buffer") {
        std::string buffer = load_data("repeated_packed_double/data-many");

        for (size_t i=1; i < buffer.size(); ++i) {
            protozero::pbf_reader item(buffer.data(), i);
            REQUIRE(item.next());
            REQUIRE_THROWS_AS(item.get_packed_double(), protozero::end_of_buffer_exception);
        }
    }

}

TEST_CASE("write repeated packed double field") {

    std::string buffer;
    protozero::pbf_writer pw(buffer);

    SECTION("empty") {
        double data[] = { 17.34 };
        pw.add_packed_double(1, std::begin(data), std::begin(data) /* !!!! */);

        REQUIRE(buffer == load_data("repeated_packed_double/data-empty"));
    }

    SECTION("one") {
        double data[] = { 17.34 };
        pw.add_packed_double(1, std::begin(data), std::end(data));

        REQUIRE(buffer == load_data("repeated_packed_double/data-one"));
    }

    SECTION("many") {
        double data[] = { 17.34, 0.0, 1.0, std::numeric_limits<double>::min(), std::numeric_limits<double>::max() };
        pw.add_packed_double(1, std::begin(data), std::end(data));

        REQUIRE(buffer == load_data("repeated_packed_double/data-many"));
    }

}

