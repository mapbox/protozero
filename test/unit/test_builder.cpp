
#include <test.hpp>

#include <string>

namespace {

enum class Outer : protozero::pbf_tag_type {
    sub = 1
};

enum class Inner : protozero::pbf_tag_type {
    value = 2
};

const std::string expected{"\x0a\x02\x10\x2a"};

} // anonymous namespace

TEST_CASE("pbf_builder submessage from pbf_builder parent with matching tag type") {
    std::string buffer;
    {
        protozero::pbf_builder<Outer> outer{buffer};
        protozero::pbf_builder<Inner> inner{outer, Outer::sub};
        inner.add_uint32(Inner::value, 42);
    }
    REQUIRE(buffer == expected);
}

TEST_CASE("pbf_builder submessage from pbf_writer parent with integer tag") {
    std::string buffer;
    {
        protozero::pbf_writer outer{buffer};
        protozero::pbf_builder<Inner> inner{outer, 1};
        inner.add_uint32(Inner::value, 42);
    }
    REQUIRE(buffer == expected);
}

TEST_CASE("pbf_builder submessage from pbf_builder parent cast to pbf_writer") {
    std::string buffer;
    {
        protozero::pbf_builder<Outer> outer{buffer};
        protozero::pbf_builder<Inner> inner{static_cast<protozero::pbf_writer&>(outer), 1};
        inner.add_uint32(Inner::value, 42);
    }
    REQUIRE(buffer == expected);
}

