
#include <test.hpp>

TEST_CASE("repeated_packed_uint32") {

    SECTION("empty") {
        std::string buffer = get_file_data("test/t/repeated_packed_uint32/data-empty.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(!item.next());
    }

    SECTION("one") {
        std::string buffer = get_file_data("test/t/repeated_packed_uint32/data-one.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        auto it_pair = item.packed_uint32();
        REQUIRE(!item.next());

        REQUIRE(*it_pair.first == 17l);
        REQUIRE(++it_pair.first == it_pair.second);
    }

    SECTION("many") {
        std::string buffer = get_file_data("test/t/repeated_packed_uint32/data-many.bin");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        REQUIRE(item.next());
        auto it_pair = item.packed_uint32();
        REQUIRE(!item.next());

        auto it = it_pair.first;
        REQUIRE(*it++ == 17l);
        REQUIRE(*it++ == 0l);
        REQUIRE(*it++ == 1l);
        REQUIRE(*it++ == std::numeric_limits<uint32_t>::max());
        REQUIRE(it == it_pair.second);
    }

}

