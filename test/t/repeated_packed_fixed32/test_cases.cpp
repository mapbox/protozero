
#include <test.hpp>

TEST_CASE("read repeated packed fixed32 field") {

    // Run these tests twice, the second time we basically move the data
    // one byte down in the buffer. It doesn't matter how the data or buffer
    // is aligned before that, in at least one of these cases the int32s will
    // not be aligned properly. So we test that even in that case the int32s
    // will be extracted properly.

    for (std::string::size_type n = 0; n < 2; ++n) {

        std::string abuffer;
        abuffer.reserve(1000);
        abuffer.append(n, '\0');

        SECTION("empty") {
            abuffer.append(load_data("repeated_packed_fixed32/data-empty"));
            protozero::pbf_reader item(abuffer.data() + n, abuffer.size() - n);

            REQUIRE(!item.next());
        }

        SECTION("one") {
            abuffer.append(load_data("repeated_packed_fixed32/data-one"));
            protozero::pbf_reader item(abuffer.data() + n, abuffer.size() - n);

            REQUIRE(item.next());
            auto it_range = item.get_packed_fixed32();
            REQUIRE(!item.next());

            REQUIRE(*it_range.begin() == 17UL);
            REQUIRE(std::next(it_range.begin()) == it_range.end());
        }

        SECTION("many") {
            abuffer.append(load_data("repeated_packed_fixed32/data-many"));
            protozero::pbf_reader item(abuffer.data() + n, abuffer.size() - n);

            REQUIRE(item.next());
            auto it_range = item.get_packed_fixed32();
            REQUIRE(!item.next());

            auto it = it_range.begin();
            REQUIRE(*it++ == 17UL);
            REQUIRE(*it++ ==  0UL);
            REQUIRE(*it++ ==  1UL);
            REQUIRE(*it++ == std::numeric_limits<uint32_t>::max());
            REQUIRE(it == it_range.end());
        }

        SECTION("end_of_buffer") {
            abuffer.append(load_data("repeated_packed_fixed32/data-many"));

            for (std::string::size_type i = 1; i < abuffer.size() - n; ++i) {
                protozero::pbf_reader item(abuffer.data() + n, i);
                REQUIRE(item.next());
                REQUIRE_THROWS_AS(item.get_packed_fixed32(), protozero::end_of_buffer_exception);
            }
        }

    }

}

TEST_CASE("write repeated packed fixed32 field") {

    std::string buffer;
    protozero::pbf_writer pw(buffer);

    SECTION("empty") {
        uint32_t data[] = { 17UL };
        pw.add_packed_fixed32(1, std::begin(data), std::begin(data) /* !!!! */);

        REQUIRE(buffer == load_data("repeated_packed_fixed32/data-empty"));
    }

    SECTION("one") {
        uint32_t data[] = { 17UL };
        pw.add_packed_fixed32(1, std::begin(data), std::end(data));

        REQUIRE(buffer == load_data("repeated_packed_fixed32/data-one"));
    }

    SECTION("many") {
        uint32_t data[] = { 17UL, 0UL, 1UL, std::numeric_limits<uint32_t>::max() };
        pw.add_packed_fixed32(1, std::begin(data), std::end(data));

        REQUIRE(buffer == load_data("repeated_packed_fixed32/data-many"));
    }

}

TEST_CASE("write repeated packed fixed32 field using packed_field_fixed32") {

    std::string buffer;
    protozero::pbf_writer pw(buffer);

    SECTION("empty - should do rollback") {
        {
            protozero::packed_field_fixed32 field{pw, 1};
        }

        REQUIRE(buffer == load_data("repeated_packed_fixed32/data-empty"));
    }

    SECTION("one") {
        {
            protozero::packed_field_fixed32 field{pw, 1};
            field.add_element(17UL);
        }

        REQUIRE(buffer == load_data("repeated_packed_fixed32/data-one"));
    }

    SECTION("one with predefined size") {
        {
            protozero::packed_field_fixed32 field{pw, 1, 1};
            field.add_element(17UL);
        }

        REQUIRE(buffer == load_data("repeated_packed_fixed32/data-one"));
    }

    SECTION("many") {
        {
            protozero::packed_field_fixed32 field{pw, 1};
            field.add_element(17UL);
            field.add_element(0UL);
            field.add_element(1UL);
            field.add_element(std::numeric_limits<uint32_t>::max());
        }

        REQUIRE(buffer == load_data("repeated_packed_fixed32/data-many"));
    }

    SECTION("many with predefined size") {
        {
            protozero::packed_field_fixed32 field{pw, 1, 4};
            field.add_element(17UL);
            field.add_element(0UL);
            field.add_element(1UL);
            field.add_element(std::numeric_limits<uint32_t>::max());
        }

        REQUIRE(buffer == load_data("repeated_packed_fixed32/data-many"));
    }

    SECTION("failure when not closing properly after zero elements") {
        protozero::packed_field_fixed32 field{pw, 1};
        REQUIRE_THROWS_AS({
            pw.add_fixed32(2, 1234); // dummy values
        }, assert_error);
    }

    SECTION("failure when not closing properly after one element") {
        protozero::packed_field_fixed32 field{pw, 1};
        field.add_element(17UL);
        REQUIRE_THROWS_AS({
            pw.add_fixed32(2, 1234); // dummy values
        }, assert_error);
    }

}

TEST_CASE("write from different types of iterators") {

    std::string buffer;
    protozero::pbf_writer pw(buffer);

    SECTION("from uint16_t") {
        uint16_t data[] = { 1, 4, 9, 16, 25 };

        pw.add_packed_fixed32(1, std::begin(data), std::end(data));
    }

    SECTION("from string") {
        std::string data = "1 4 9 16 25";
        std::stringstream sdata(data);

        std::istream_iterator<uint32_t> eod;
        std::istream_iterator<uint32_t> it(sdata);

        pw.add_packed_fixed32(1, it, eod);
    }

    protozero::pbf_reader item(buffer);

    REQUIRE(item.next());
    auto it_range = item.get_packed_fixed32();
    REQUIRE(!item.next());
    REQUIRE(std::distance(it_range.begin(), it_range.end()) == 5);

    auto i = it_range.begin();
    REQUIRE(*i++ ==  1);
    REQUIRE(*i++ ==  4);
    REQUIRE(*i++ ==  9);
    REQUIRE(*i++ == 16);
    REQUIRE(*i++ == 25);

}

