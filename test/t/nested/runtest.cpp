
#include <test.hpp>

inline void check_subsub(mapbox::util::pbf message) {
    while (message.next()) {
        switch (message.tag()) {
            case 1: {
                REQUIRE("foobar" == message.get_string());
                break;
            }
            case 2: {
                REQUIRE(99 == message.get_int32());
                break;
            }
            default: {
                REQUIRE(false); // should never be here
                break;
            }
        }
    }
}

inline void check_sub(mapbox::util::pbf message) {
    while (message.next()) {
        switch (message.tag()) {
            case 1: {
                check_subsub(message.get_message());
                break;
            }
            case 2: {
                REQUIRE(88 == message.get_int32());
                break;
            }
            default: {
                REQUIRE(false); // should never be here
                break;
            }
        }
    }
}

inline void check(mapbox::util::pbf message) {
    while (message.next()) {
        switch (message.tag()) {
            case 1: {
                check_sub(message.get_message());
                break;
            }
            case 2: {
                REQUIRE(77 == message.get_int32());
                break;
            }
            default: {
                REQUIRE(false); // should never be here
                break;
            }
        }
    }
}

TEST_CASE("read nested message fields") {

    SECTION("string") {
        std::string buffer = load_data("nested/data-message");

        mapbox::util::pbf message(buffer.data(), buffer.size());
        check(message);
    }

}

TEST_CASE("write nested message fields") {

    std::string buffer;
    mapbox::util::pbf_writer pw(buffer);

    SECTION("string") {
        std::string ssbuffer;
        mapbox::util::pbf_writer pwss(ssbuffer);
        pwss.add_string(1, "foobar");
        pwss.add_int32(2, 99);

        std::string sbuffer;
        mapbox::util::pbf_writer pws(sbuffer);
        pws.add_string(1, ssbuffer);
        pws.add_int32(2, 88);

        pw.add_message(1, sbuffer);
    }

    SECTION("string with subwriter") {
        mapbox::util::pbf_subwriter sw(pw, 1);
        {
            mapbox::util::pbf_subwriter ssw(pw, 1);
            pw.add_string(1, "foobar");
            pw.add_int32(2, 99);
        }
        pw.add_int32(2, 88);
    }

    pw.add_int32(2, 77);

    mapbox::util::pbf message(buffer.data(), buffer.size());
    check(message);
}

