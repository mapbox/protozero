
#include <test.hpp>

TEST_CASE("complex") {

    SECTION("minimal") {
        std::string buffer = get_file_data("test/t/complex/data-minimal.pbf");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        while (item.next()) {
            switch (item.tag()) {
                case 1: {
                    REQUIRE(item.get_fixed32() == 12345678L);
                    break;
                }
                case 5: {
                    mapbox::util::pbf subitem = item.get_message();
                    REQUIRE(subitem.next());
                    REQUIRE(subitem.get_string() == "foobar");
                    REQUIRE(!subitem.next());
                    break;
                }
                default: {
                    REQUIRE(false); // should not be here
                    break;
                }
            }
        }
    }

    SECTION("some") {
        std::string buffer = get_file_data("test/t/complex/data-some.pbf");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        uint32_t sum_of_u = 0;
        while (item.next()) {
            switch (item.tag()) {
                case 1: {
                    REQUIRE(item.get_fixed32() == 12345678L);
                    break;
                }
                case 2: {
                    REQUIRE(true);
                    item.skip();
                    break;
                }
                case 4: {
                    sum_of_u += item.get_uint32();
                    break;
                }
                case 5: {
                    mapbox::util::pbf subitem = item.get_message();
                    REQUIRE(subitem.next());
                    REQUIRE(subitem.get_string() == "foobar");
                    REQUIRE(!subitem.next());
                    break;
                }
                default: {
                    REQUIRE(false); // should not be here
                    break;
                }
            }
        }
        REQUIRE(sum_of_u == 66);
    }

    SECTION("all") {
        std::string buffer = get_file_data("test/t/complex/data-all.pbf");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        int number_of_u = 0;
        while (item.next()) {
            switch (item.tag()) {
                case 1: {
                    REQUIRE(item.get_fixed32() == 12345678L);
                    break;
                }
                case 2: {
                    REQUIRE(true);
                    item.skip();
                    break;
                }
                case 3: {
                    REQUIRE(item.get_int64() == 555555555LL);
                    break;
                }
                case 4: {
                    item.skip();
                    ++number_of_u;
                    break;
                }
                case 5: {
                    mapbox::util::pbf subitem = item.get_message();
                    REQUIRE(subitem.next());
                    REQUIRE(subitem.get_string() == "foobar");
                    REQUIRE(!subitem.next());
                    break;
                }
                case 7: {
                    auto pi = item.packed_sint32();
                    int32_t sum = 0;
                    for (auto it = pi.first; it != pi.second; ++it) {
                        sum += *it;
                    }
                    REQUIRE(sum == 5);
                    break;
                }
                case 8: {
                    REQUIRE(item.get_string() == "optionalstring");
                    break;
                }
                default: {
                    REQUIRE(false); // should not be here
                    break;
                }
            }
        }
        REQUIRE(number_of_u == 5);
    }

}

