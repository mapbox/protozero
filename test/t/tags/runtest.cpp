
#include <test.hpp>

inline void check_tag(const std::string& buffer, mapbox::util::pbf_tag_type tag) {
    mapbox::util::pbf item(buffer.data(), buffer.size());

    REQUIRE(item.next());
    REQUIRE(item.tag() == tag);
    REQUIRE(item.get_int32() == 333L);
    REQUIRE(!item.next());
}

TEST_CASE("tags") {

    SECTION("tag 1") {
        check_tag(get_file_data("test/t/tags/data-tag-1.pbf"), 1L);
    }

    SECTION("tag 200") {
        check_tag(get_file_data("test/t/tags/data-tag-200.pbf"), 200L);
    }

    SECTION("tag 200000") {
        check_tag(get_file_data("test/t/tags/data-tag-200000.pbf"), 200000L);
    }

    SECTION("tag max") {
        check_tag(get_file_data("test/t/tags/data-tag-max.pbf"), (1L << 29) - 1);
    }

}

TEST_CASE("write tags") {

    std::string buffer;
    std::string wbuffer;
    mapbox::util::pbf_writer pw(wbuffer);

    SECTION("tag 1") {
        buffer = get_file_data("test/t/tags/data-tag-1.pbf");
        pw.add_int32(1L, 333L);
    }

    SECTION("tag 200") {
        buffer = get_file_data("test/t/tags/data-tag-200.pbf");
        pw.add_int32(200L, 333L);
    }

    SECTION("tag 200000") {
        buffer = get_file_data("test/t/tags/data-tag-200000.pbf");
        pw.add_int32(200000L, 333L);
    }

    SECTION("tag max") {
        buffer = get_file_data("test/t/tags/data-tag-max.pbf");
        pw.add_int32((1L << 29) - 1, 333L);
    }

    REQUIRE(buffer == wbuffer);

}

