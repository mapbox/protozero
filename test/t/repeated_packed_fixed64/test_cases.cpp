
#include <test.hpp>

TEST_CASE("read repeated packed fixed64 field") {

    // Run these tests twice, the second time we basically move the data
    // one byte down in the buffer. It doesn't matter how the data or buffer
    // is aligned before that, in at least one of these cases the int64s will
    // not be aligned properly. So we test that even in that case the int64s
    // will be extracted properly.

    for (std::string::size_type n = 0; n < 2; ++n) {

        std::string abuffer;
        abuffer.reserve(1000);
        abuffer.append(n, '\0');

        SECTION("empty") {
            abuffer.append(load_data("repeated_packed_fixed64/data-empty"));
            protozero::pbf_reader item(abuffer.data() + n, abuffer.size() - n);

            REQUIRE(!item.next());
        }

        SECTION("one") {
            abuffer.append(load_data("repeated_packed_fixed64/data-one"));
            protozero::pbf_reader item(abuffer.data() + n, abuffer.size() - n);

            REQUIRE(item.next());
            auto it_pair = item.get_packed_fixed64();
            REQUIRE(!item.next());

            REQUIRE(*it_pair.first == 17ULL);
            REQUIRE(++it_pair.first == it_pair.second);
        }

        SECTION("many") {
            abuffer.append(load_data("repeated_packed_fixed64/data-many"));
            protozero::pbf_reader item(abuffer.data() + n, abuffer.size() - n);

            REQUIRE(item.next());
            auto it_pair = item.get_packed_fixed64();
            REQUIRE(!item.next());

            auto it = it_pair.first;
            REQUIRE(*it++ == 17ULL);
            REQUIRE(*it++ ==  0ULL);
            REQUIRE(*it++ ==  1ULL);
            REQUIRE(*it++ == std::numeric_limits<uint64_t>::max());
            REQUIRE(it == it_pair.second);
        }

        SECTION("end_of_buffer") {
            abuffer.append(load_data("repeated_packed_fixed64/data-many"));

            for (std::string::size_type i = 1; i < abuffer.size() - n; ++i) {
                protozero::pbf_reader item(abuffer.data() + n, i);
                REQUIRE(item.next());
                REQUIRE_THROWS_AS(item.get_packed_fixed64(), protozero::end_of_buffer_exception);
            }
        }

    }

}

TEST_CASE("write repeated packed fixed64 field") {

    std::string buffer;
    protozero::pbf_writer pw(buffer);

    SECTION("empty") {
        uint64_t data[] = { 17ULL };
        pw.add_packed_fixed64(1, std::begin(data), std::begin(data) /* !!!! */);

        REQUIRE(buffer == load_data("repeated_packed_fixed64/data-empty"));
    }

    SECTION("one") {
        uint64_t data[] = { 17ULL };
        pw.add_packed_fixed64(1, std::begin(data), std::end(data));

        REQUIRE(buffer == load_data("repeated_packed_fixed64/data-one"));
    }

    SECTION("many") {
        uint64_t data[] = { 17ULL, 0ULL, 1ULL, std::numeric_limits<uint64_t>::max() };
        pw.add_packed_fixed64(1, std::begin(data), std::end(data));

        REQUIRE(buffer == load_data("repeated_packed_fixed64/data-many"));
    }

}

