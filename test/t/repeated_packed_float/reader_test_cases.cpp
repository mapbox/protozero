
#include <test.hpp>

TEST_CASE("read repeated packed float field") {

    SECTION("empty") {
        std::string buffer = load_data("repeated_packed_float/data-empty");

        protozero::pbf_reader item(buffer.data(), buffer.size());

        REQUIRE(!item.next());
    }

    SECTION("one") {
        std::string buffer = load_data("repeated_packed_float/data-one");

        protozero::pbf_reader item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        auto it_pair = item.get_packed_float();
        REQUIRE(!item.next());

        REQUIRE(*it_pair.first == 17.34f);
        REQUIRE(++it_pair.first == it_pair.second);
    }

    SECTION("many") {
        std::string buffer = load_data("repeated_packed_float/data-many");

        protozero::pbf_reader item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        auto it_pair = item.get_packed_float();
        REQUIRE(!item.next());

        auto it = it_pair.first;
        REQUIRE(*it++ == 17.34f);
        REQUIRE(*it++ ==   0.0f);
        REQUIRE(*it++ ==   1.0f);
        REQUIRE(*it++ == std::numeric_limits<float>::min());
        REQUIRE(*it++ == std::numeric_limits<float>::max());
        REQUIRE(it == it_pair.second);
    }

    SECTION("end_of_buffer") {
        std::string buffer = load_data("repeated_packed_float/data-many");

        for (size_t i=1; i < buffer.size(); ++i) {
            protozero::pbf_reader item(buffer.data(), i);
            REQUIRE(item.next());
            REQUIRE_THROWS_AS(item.get_packed_float(), protozero::end_of_buffer_exception);
        }
    }

}

TEST_CASE("write repeated packed float field") {

    std::string buffer;
    protozero::pbf_writer pw(buffer);

    SECTION("empty") {
        float data[] = { 17.34f };
        pw.add_packed_float(1, std::begin(data), std::begin(data) /* !!!! */);

        REQUIRE(buffer == load_data("repeated_packed_float/data-empty"));
    }

    SECTION("one") {
        float data[] = { 17.34f };
        pw.add_packed_float(1, std::begin(data), std::end(data));

        REQUIRE(buffer == load_data("repeated_packed_float/data-one"));
    }

    SECTION("many") {
        float data[] = { 17.34f, 0.0f, 1.0f, std::numeric_limits<float>::min(), std::numeric_limits<float>::max() };
        pw.add_packed_float(1, std::begin(data), std::end(data));

        REQUIRE(buffer == load_data("repeated_packed_float/data-many"));
    }

}

