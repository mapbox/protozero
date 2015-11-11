
#include <test.hpp>

TEST_CASE("read fixed64 field") {

    // Run these tests twice, the second time we basically move the data
    // one byte down in the buffer. It doesn't matter how the data or buffer
    // is aligned before that, in at least one of these cases the int64 will
    // not be aligned properly. So we test that even in that case the int64
    // will be extracted properly.

    for (std::string::size_type n = 0; n < 2; ++n) {

        std::string abuffer;
        abuffer.reserve(1000);
        abuffer.append(n, '\0');

        SECTION("zero") {
            abuffer.append(load_data("fixed64/data-zero"));
            protozero::pbf_reader item(abuffer.data() + n, abuffer.size() - n);

            REQUIRE(item.next());
            REQUIRE(item.get_fixed64() == 0ULL);
            REQUIRE(!item.next());
        }

        SECTION("max-uint") {
            abuffer.append(load_data("fixed64/data-max-uint"));
            protozero::pbf_reader item(abuffer.data() + n, abuffer.size() - n);

            REQUIRE(item.next());
            REQUIRE(item.get_fixed64() == std::numeric_limits<uint64_t>::max());
            REQUIRE(!item.next());
        }

        SECTION("min-uint") {
            abuffer.append(load_data("fixed64/data-min-uint"));
            protozero::pbf_reader item(abuffer.data() + n, abuffer.size() - n);

            REQUIRE(item.next());
            REQUIRE(item.get_fixed64() == std::numeric_limits<uint64_t>::min());
            REQUIRE(!item.next());
        }

        SECTION("end_of_buffer") {
            abuffer.append(load_data("fixed64/data-min-uint"));

            for (std::string::size_type i = 1; i < abuffer.size() - n; ++i) {
                protozero::pbf_reader item(abuffer.data() + n, i);
                REQUIRE(item.next());
                REQUIRE_THROWS_AS(item.get_fixed64(), protozero::end_of_buffer_exception);
            }
        }

    }

}

TEST_CASE("write fixed64 field") {

    std::string buffer;
    protozero::pbf_writer pw(buffer);

    SECTION("zero") {
        pw.add_fixed64(1, 0ULL);
        REQUIRE(buffer == load_data("fixed64/data-zero"));
    }

    SECTION("max-uint") {
        pw.add_fixed64(1, std::numeric_limits<uint64_t>::max());
        REQUIRE(buffer == load_data("fixed64/data-max-uint"));
    }

    SECTION("min-uint") {
        pw.add_fixed64(1, std::numeric_limits<uint64_t>::min());
        REQUIRE(buffer == load_data("fixed64/data-min-uint"));
    }

}

