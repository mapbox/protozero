
#include <test.hpp>

inline void check_subsub(protozero::pbf_reader message) {
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

inline void check_sub(protozero::pbf_reader message) {
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

inline void check(protozero::pbf_reader message) {
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

        protozero::pbf_reader message(buffer.data(), buffer.size());
        check(message);
    }

}

TEST_CASE("write nested message fields") {

    std::string buffer;
    protozero::pbf_writer pw(buffer);

    SECTION("string") {
        std::string ssbuffer;
        protozero::pbf_writer pwss(ssbuffer);
        pwss.add_string(1, "foobar");
        pwss.add_int32(2, 99);

        std::string sbuffer;
        protozero::pbf_writer pws(sbuffer);
        pws.add_string(1, ssbuffer);
        pws.add_int32(2, 88);

        pw.add_message(1, sbuffer);
    }

    SECTION("string with subwriter") {
        protozero::pbf_subwriter sw(pw, 1);
        {
            protozero::pbf_subwriter ssw(pw, 1);
            pw.add_string(1, "foobar");
            pw.add_int32(2, 99);
        }
        pw.add_int32(2, 88);
    }

    pw.add_int32(2, 77);

    protozero::pbf_reader message(buffer.data(), buffer.size());
    check(message);
}

