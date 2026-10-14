#include <test.hpp>

#define PBF_TYPE fixed32
#define PBF_TYPE_IS_SIGNED 0
using cpp_type = uint32_t;

#include <packed_access.hpp>

// Closing a packed field with known size must reset the parent writer so it
// can be written to again.
TEST_CASE("write repeated packed fixed32 field using packed field with known size") {
    std::string buffer;
    protozero::pbf_writer pw{buffer};

    // field 1, length 8, fixed32 values 17 and 1
    const std::string packed{"\x0a\x08\x11\x00\x00\x00\x01\x00\x00\x00", 10};
    // field 2, varint 5
    const std::string scalar{"\x10\x05", 2};

    SECTION("closed by destructor, then add field to parent") {
        {
            protozero::packed_field_fixed32 field{pw, 1, 2};
            field.add_element(17);
            field.add_element(1);
        }
        pw.add_uint32(2, 5);

        REQUIRE(buffer == packed + scalar);
    }

    SECTION("closed by commit(), then add field to parent") {
        protozero::packed_field_fixed32 field{pw, 1, 2};
        field.add_element(17);
        field.add_element(1);
        field.commit();
        pw.add_uint32(2, 5);

        REQUIRE(buffer == packed + scalar);
    }

    SECTION("two packed fields in a row") {
        {
            protozero::packed_field_fixed32 field{pw, 1, 2};
            field.add_element(17);
            field.add_element(1);
        }
        {
            protozero::packed_field_fixed32 field{pw, 1, 2};
            field.add_element(17);
            field.add_element(1);
        }

        REQUIRE(buffer == packed + packed);
    }

    SECTION("inside a submessage that is committed afterwards") {
        protozero::pbf_writer sw{pw, 3};
        {
            protozero::packed_field_fixed32 field{sw, 1, 2};
            field.add_element(17);
            field.add_element(1);
        }
        sw.commit();

        // field 3, length 10, containing the packed field
        REQUIRE(buffer == std::string{"\x1a\x0a", 2} + packed);
    }
}
