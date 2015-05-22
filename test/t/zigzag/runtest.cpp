
#include <test.hpp>

using mapbox::util::pbf;
using mapbox::util::pbf_writer;

inline int32_t zz32(int32_t val) {
    return pbf::decode_zigzag32(pbf_writer::encode_zigzag32(val));
}

inline int64_t zz64(int64_t val) {
    return pbf::decode_zigzag64(pbf_writer::encode_zigzag64(val));
}

TEST_CASE("zigzag") {

    SECTION("some values - 32bit") {

        REQUIRE(pbf_writer::encode_zigzag32( 0L) == 0UL);
        REQUIRE(pbf_writer::encode_zigzag32(-1L) == 1UL);
        REQUIRE(pbf_writer::encode_zigzag32( 1L) == 2UL);
        REQUIRE(pbf_writer::encode_zigzag32(-2L) == 3UL);
        REQUIRE(pbf_writer::encode_zigzag32( 2L) == 4UL);

    }

    SECTION("some values - 64bit") {

        REQUIRE(pbf_writer::encode_zigzag64( 0LL) == 0ULL);
        REQUIRE(pbf_writer::encode_zigzag64(-1LL) == 1ULL);
        REQUIRE(pbf_writer::encode_zigzag64( 1LL) == 2ULL);
        REQUIRE(pbf_writer::encode_zigzag64(-2LL) == 3ULL);
        REQUIRE(pbf_writer::encode_zigzag64( 2LL) == 4ULL);

    }

    SECTION("zigzag and back - 32bit") {
        REQUIRE(zz32( 0L) ==  0L);
        REQUIRE(zz32( 1L) ==  1L);
        REQUIRE(zz32(-1L) == -1L);
        REQUIRE(zz32(std::numeric_limits<int32_t>::max()) == std::numeric_limits<int32_t>::max());
        REQUIRE(zz32(std::numeric_limits<int32_t>::min()) == std::numeric_limits<int32_t>::min());
    }

    SECTION("zigzag and back - 64bit") {
        REQUIRE(zz64( 0LL) ==  0LL);
        REQUIRE(zz64( 1LL) ==  1LL);
        REQUIRE(zz64(-1LL) == -1LL);
        REQUIRE(zz64(std::numeric_limits<int64_t>::max()) == std::numeric_limits<int64_t>::max());
        REQUIRE(zz64(std::numeric_limits<int64_t>::min()) == std::numeric_limits<int64_t>::min());
    }

}

