
#include <test.hpp>

TEST_CASE("skip") {

    SECTION("check that skip skips the right amount of bytes") {

        std::vector<std::string> filenames = {
            "bool/data-true",
            "bool/data-false",
            "bytes/data-one",
            "bytes/data-string",
            "bytes/data-binary",
            "double/data-zero",
            "double/data-pos",
            "double/data-neg",
            "float/data-zero",
            "float/data-pos",
            "float/data-neg",
            "fixed32/data-zero",
            "fixed32/data-max-uint",
            "fixed32/data-min-uint",
            "fixed64/data-zero",
            "fixed64/data-max-uint",
            "fixed64/data-min-uint",
            "sfixed32/data-zero",
            "sfixed32/data-max-int",
            "sfixed32/data-min-int",
            "sfixed64/data-zero",
            "sfixed64/data-max-int",
            "sfixed64/data-min-int",
            "message/data-message",
            "string/data-one",
            "string/data-string",
            "int32/data-zero",
            "int32/data-pos",
            "int32/data-neg",
            "int32/data-min",
            "int32/data-max",
            "int64/data-zero",
            "int64/data-pos",
            "int64/data-neg",
            "int64/data-min",
            "int64/data-max",
            "sint32/data-zero",
            "sint32/data-pos",
            "sint32/data-neg",
            "sint32/data-min",
            "sint32/data-max",
            "sint64/data-zero",
            "sint64/data-pos",
            "sint64/data-neg",
            "sint64/data-min",
            "sint64/data-max",
            "uint32/data-zero",
            "uint32/data-pos",
            "uint32/data-max",
            "uint64/data-zero",
            "uint64/data-pos",
            "repeated/data-one",
        };

        for (const auto& filename : filenames) {
            std::string buffer = get_file_data(std::string("test/t/") + filename + ".pbf");

            mapbox::util::pbf item(buffer.data(), buffer.size());

            REQUIRE(item.next());
            item.skip();
            REQUIRE(!item);
        }

    }

}

