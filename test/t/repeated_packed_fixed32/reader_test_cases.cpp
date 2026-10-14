
#include <test.hpp>

#define PBF_TYPE fixed32
#define PBF_TYPE_IS_SIGNED 0
using cpp_type = uint32_t;

#include <packed_access.hpp>


TEST_CASE("write repeated packed fixed32 field using packed field with known size") {
    std::string buffer;
    protozero::pbf_writer pw{buffer};

    SECTION("correct number of elements") {
        {
            protozero::packed_field_fixed32 field{pw, 1, 2};
            field.add_element(17);
            field.add_element(1);
        }
        pw.add_uint32(2, 5);

        REQUIRE(buffer == std::string{"\x0a\x08\x11\x00\x00\x00\x01\x00\x00\x00\x10\x05", 12});
    }

    SECTION("correct number of elements with commit") {
        protozero::packed_field_fixed32 field{pw, 1, 2};
        field.add_element(17);
        field.add_element(1);
        field.commit();
        pw.add_uint32(2, 5);

        REQUIRE(buffer == std::string{"\x0a\x08\x11\x00\x00\x00\x01\x00\x00\x00\x10\x05", 12});
    }

    SECTION("too few elements") {
        protozero::packed_field_fixed32 field{pw, 1, 2};
        field.add_element(17);
        REQUIRE_THROWS_AS(field.commit(), assert_error);
    }

    SECTION("too many elements") {
        protozero::packed_field_fixed32 field{pw, 1, 2};
        field.add_element(17);
        field.add_element(1);
        field.add_element(2);
        REQUIRE_THROWS_AS(field.commit(), assert_error);
    }
}

TEST_CASE("write submessage with known size") {
    std::string buffer;
    protozero::pbf_writer pw{buffer};

    {
        protozero::pbf_writer sw{pw, 1, 2};
        sw.add_uint32(1, 5);
    }
    pw.add_uint32(2, 5);

    REQUIRE(buffer == std::string{"\x0a\x02\x08\x05\x10\x05", 6});
}
