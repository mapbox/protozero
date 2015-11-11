
#include <test.hpp>

TEST_CASE("read fixed32 field") {

    // Run these tests twice, the second time we basically move the data
    // one byte down in the buffer. It doesn't matter how the data or buffer
    // is aligned before that, in at least one of these cases the int32 will
    // not be aligned properly. So we test that even in that case the int32
    // will be extracted properly.

    for (std::string::size_type n = 0; n < 2; ++n) {

        std::string abuffer;
        abuffer.reserve(1000);
        abuffer.append(n, '\0');

        SECTION("zero") {
            abuffer.append(load_data("fixed32/data-zero"));
            protozero::pbf_reader item(abuffer.data() + n, abuffer.size() - n);

            REQUIRE(item.next());
            REQUIRE(item.get_fixed32() == 0UL);
            REQUIRE(!item.next());
        }

        SECTION("max-uint") {
            abuffer.append(load_data("fixed32/data-max-uint"));
            protozero::pbf_reader item(abuffer.data() + n, abuffer.size() - n);

            REQUIRE(item.next());
            REQUIRE(item.get_fixed32() == std::numeric_limits<uint32_t>::max());
            REQUIRE(!item.next());
        }

        SECTION("min-uint") {
            abuffer.append(load_data("fixed32/data-min-uint"));
            protozero::pbf_reader item(abuffer.data() + n, abuffer.size() - n);

            REQUIRE(item.next());
            REQUIRE(item.get_fixed32() == std::numeric_limits<uint32_t>::min());
            REQUIRE(!item.next());
        }

        SECTION("end_of_buffer") {
            abuffer.append(load_data("fixed32/data-min-uint"));

            for (std::string::size_type i = 1; i < abuffer.size() - n; ++i) {
                protozero::pbf_reader item(abuffer.data() + n, i);
                REQUIRE(item.next());
                REQUIRE_THROWS_AS(item.get_fixed32(), protozero::end_of_buffer_exception);
            }
        }

        SECTION("check assert on varint/string access") {
            abuffer.append(load_data("fixed32/data-zero"));
            protozero::pbf_reader item(abuffer.data() + n, abuffer.size() - n);

            REQUIRE(item.next());
            REQUIRE_THROWS_AS(item.get_string(), assert_error);
        }

        SECTION("assert detecting tag==0") {
            abuffer.append(load_data("fixed32/data-zero"));
            protozero::pbf_reader item(abuffer.data() + n, abuffer.size() - n);

            REQUIRE_THROWS_AS(item.get_fixed32(), assert_error);
            REQUIRE(item.next());
            REQUIRE(item.get_fixed32() == 0UL);
            REQUIRE_THROWS(item.get_fixed32());
            REQUIRE(!item.next());
        }

        SECTION("skip") {
            abuffer.append(load_data("fixed32/data-zero"));
            protozero::pbf_reader item(abuffer.data() + n, abuffer.size() - n);

            REQUIRE_THROWS_AS(item.skip(), assert_error);
            REQUIRE(item.next());
            item.skip();
            REQUIRE_THROWS(item.skip());
            REQUIRE(!item.next());
        }

    }

}

TEST_CASE("write fixed32 field") {

    std::string buffer;
    protozero::pbf_writer pw(buffer);

    SECTION("zero") {
        pw.add_fixed32(1, 0);
        REQUIRE(buffer == load_data("fixed32/data-zero"));
    }

    SECTION("max-uint") {
        pw.add_fixed32(1, std::numeric_limits<uint32_t>::max());
        REQUIRE(buffer == load_data("fixed32/data-max-uint"));
    }

    SECTION("min-uint") {
        pw.add_fixed32(1, std::numeric_limits<uint32_t>::min());
        REQUIRE(buffer == load_data("fixed32/data-min-uint"));
    }

}

