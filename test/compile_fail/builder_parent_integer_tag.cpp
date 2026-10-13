
// This file must not compile: A pbf_builder parent requires a tag of its enum
// type, not a plain integer.

#include <protozero/pbf_builder.hpp>

#include <string>

enum class Outer : protozero::pbf_tag_type {
    sub = 1
};

enum class Inner : protozero::pbf_tag_type {
    value = 2
};

void test() {
    std::string buffer;
    protozero::pbf_builder<Outer> outer{buffer};
    protozero::pbf_builder<Inner> inner{outer, 1};
}

