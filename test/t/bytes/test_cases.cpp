
#include <test.hpp>

TEST_CASE("read bytes field") {

    SECTION("empty") {
        const std::string buffer = load_data("bytes/data-empty");

        protozero::pbf_reader item(buffer);

        REQUIRE(item.next());
        REQUIRE(item.get_string() == "");
        REQUIRE(!item.next());
    }

    SECTION("one") {
        const std::string buffer = load_data("bytes/data-one");

        protozero::pbf_reader item(buffer);

        REQUIRE(item.next());
        REQUIRE(item.get_string() == "x");
        REQUIRE(!item.next());
    }

    SECTION("string") {
        const std::string buffer = load_data("bytes/data-string");

        protozero::pbf_reader item(buffer);

        REQUIRE(item.next());
        REQUIRE(item.get_string() == "foobar");
        REQUIRE(!item.next());
    }

    SECTION("binary") {
        const std::string buffer = load_data("bytes/data-binary");

        protozero::pbf_reader item(buffer);

        REQUIRE(item.next());
        const std::string data = item.get_string();
        REQUIRE(data.size() == 3);
        REQUIRE(data[0] == char(1));
        REQUIRE(data[1] == char(2));
        REQUIRE(data[2] == char(3));
        REQUIRE(!item.next());
    }

    SECTION("end_of_buffer") {
        const std::string buffer = load_data("bytes/data-binary");

        for (std::string::size_type i = 1; i < buffer.size(); ++i) {
            protozero::pbf_reader item(buffer.data(), i);
            REQUIRE(item.next());
            REQUIRE_THROWS_AS(item.get_string(), protozero::end_of_buffer_exception);
        }
    }

}

TEST_CASE("write bytes field") {

    std::string buffer;
    protozero::pbf_writer pw(buffer);

    SECTION("empty") {
        pw.add_string(1, "");
        REQUIRE(buffer == load_data("bytes/data-empty"));
    }

    SECTION("one") {
        pw.add_string(1, "x");
        REQUIRE(buffer == load_data("bytes/data-one"));
    }

    SECTION("string") {
        pw.add_string(1, "foobar");
        REQUIRE(buffer == load_data("bytes/data-string"));
    }

    SECTION("binary") {
        std::string data;
        data.append(1, char(1));
        data.append(1, char(2));
        data.append(1, char(3));

        pw.add_string(1, data);

        REQUIRE(buffer == load_data("bytes/data-binary"));
    }

}

TEST_CASE("write bytes field using vectored approach") {

    std::string buffer;
    protozero::pbf_writer pw(buffer);

    SECTION("string") {
        std::string d1{"foo"};
        std::string d2{"bar"};

        pw.add_bytes_vectored(1, d1, d2);
    }

    SECTION("string") {
        std::string d1{"foo"};
        std::string d2{"bar"};
        protozero::data_view dv{d2};

        pw.add_bytes_vectored(1, d1, dv);
    }

    SECTION("string") {
        std::string d1{"foo"};
        std::string d2{"ba"};
        std::string d3{"r"};

        pw.add_bytes_vectored(1, d1, d2, d3);
    }

    SECTION("string") {
        std::string d1{"foo"};
        std::string d2{""};
        std::string d3{"bar"};

        pw.add_bytes_vectored(1, d1, d2, d3);
    }

    REQUIRE(buffer == load_data("bytes/data-string"));
}

TEST_CASE("write bytes field using vectored approach with builder") {
    enum class foo : protozero::pbf_tag_type { bar = 1 };
    std::string buffer;
    protozero::pbf_builder<foo> pw(buffer);

    std::string d1{"foo"};
    std::string d2{"bar"};

    pw.add_bytes_vectored(foo::bar, d1, d2);

    REQUIRE(buffer == load_data("bytes/data-string"));
}

