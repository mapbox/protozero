
#include <test.hpp>

TEST_CASE("read complex data") {

    SECTION("minimal") {
        std::string buffer = load_data("complex/data-minimal");

        protozero::pbf_reader item(buffer);

        while (item.next()) {
            switch (item.tag()) {
                case 1: {
                    REQUIRE(item.get_fixed32() == 12345678L);
                    break;
                }
                case 5: {
                    protozero::pbf_reader subitem = item.get_message();
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
        std::string buffer = load_data("complex/data-some");

        protozero::pbf_reader item(buffer);

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
                    protozero::pbf_reader subitem = item.get_message();
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
        std::string buffer = load_data("complex/data-all");

        protozero::pbf_reader item(buffer);

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
                    protozero::pbf_reader subitem = item.get_message();
                    REQUIRE(subitem.next());
                    REQUIRE(subitem.get_string() == "foobar");
                    REQUIRE(!subitem.next());
                    break;
                }
                case 7: {
                    auto pi = item.get_packed_sint32();
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

    SECTION("skip everything") {
        std::string buffer = load_data("complex/data-all");

        protozero::pbf_reader item(buffer);

        while (item.next()) {
            switch (item.tag()) {
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 7:
                case 8:
                    item.skip();
                    break;
                default: {
                    REQUIRE(false); // should not be here
                    break;
                }
            }
        }
    }

}

TEST_CASE("write complex data") {

    SECTION("minimal") {
        std::string buffer;
        protozero::pbf_writer pw(buffer);
        pw.add_fixed32(1, 12345678);

        std::string submessage;
        protozero::pbf_writer pws(submessage);
        pws.add_string(1, "foobar");

        pw.add_message(5, submessage);

        protozero::pbf_reader item(buffer);

        while (item.next()) {
            switch (item.tag()) {
                case 1: {
                    REQUIRE(item.get_fixed32() == 12345678L);
                    break;
                }
                case 5: {
                    protozero::pbf_reader subitem = item.get_message();
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
        std::string buffer;
        protozero::pbf_writer pw(buffer);
        pw.add_fixed32(1, 12345678);

        std::string submessage;
        protozero::pbf_writer pws(submessage);
        pws.add_string(1, "foobar");

        pw.add_uint32(4, 22);
        pw.add_uint32(4, 44);
        pw.add_int64(2, -9876543);
        pw.add_message(5, submessage);

        protozero::pbf_reader item(buffer);

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
                    protozero::pbf_reader subitem = item.get_message();
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
        std::string buffer;
        protozero::pbf_writer pw(buffer);
        pw.add_fixed32(1, 12345678);

        std::string submessage;
        protozero::pbf_writer pws(submessage);
        pws.add_string(1, "foobar");
        pw.add_message(5, submessage);

        pw.add_uint32(4, 22);
        pw.add_uint32(4, 44);
        pw.add_int64(2, -9876543);
        pw.add_uint32(4, 44);
        pw.add_uint32(4, 66);
        pw.add_uint32(4, 66);

        int32_t d[] = { -17, 22 };
        pw.add_packed_sint32(7, std::begin(d), std::end(d));

        pw.add_int64(3, 555555555);

        protozero::pbf_reader item(buffer);

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
                    protozero::pbf_reader subitem = item.get_message();
                    REQUIRE(subitem.next());
                    REQUIRE(subitem.get_string() == "foobar");
                    REQUIRE(!subitem.next());
                    break;
                }
                case 7: {
                    auto pi = item.get_packed_sint32();
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

static void check_message(const std::string& buffer) {
    protozero::pbf_reader item(buffer);

    while (item.next()) {
        switch (item.tag()) {
            case 1: {
                REQUIRE(item.get_fixed32() == 42L);
                break;
            }
            case 5: {
                protozero::pbf_reader subitem = item.get_message();
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

TEST_CASE("write complex with subwriter") {
    std::string buffer_test;
    protozero::pbf_writer pbf_test(buffer_test);
    pbf_test.add_fixed32(1, 42L);

    SECTION("message in message") {
        protozero::pbf_writer pbf_submessage(pbf_test, 5);
        pbf_submessage.add_string(1, "foobar");
    }

    check_message(buffer_test);
}

