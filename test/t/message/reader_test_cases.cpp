
#include <test.hpp>

TEST_CASE("read message field: string") {
    const std::string buffer = load_data("message/data-message");

    protozero::pbf_reader item{buffer};

    REQUIRE(item.data().data() == buffer.data());
    REQUIRE(item.data().size() == buffer.size());

    REQUIRE(item.next());
    protozero::pbf_reader subitem{item.get_message()};
    REQUIRE_FALSE(item.next());

    REQUIRE(item.data().data() == buffer.data() + buffer.size());
    REQUIRE(item.data().empty());

    REQUIRE(subitem.next());
    REQUIRE(subitem.get_string() == "foobar");
    REQUIRE_FALSE(subitem.next());
}

TEST_CASE("read message field: end of buffer") {
    const std::string buffer = load_data("message/data-message");

    for (std::string::size_type i = 1; i < buffer.size(); ++i) {
        protozero::pbf_reader item{buffer.data(), i};
        REQUIRE(item.next());
        REQUIRE_THROWS_AS(item.get_string(), protozero::end_of_buffer_exception);
    }
}

TEST_CASE("read message field: optional contents of message - empty") {
    const std::string buffer = load_data("message/data-opt-empty");

    protozero::pbf_reader item{buffer};

    REQUIRE_FALSE(item.next());
}

TEST_CASE("read message field: string opt") {
    const std::string buffer = load_data("message/data-opt-element");

    protozero::pbf_reader item{buffer};

    REQUIRE(item.next());
    REQUIRE(item.get_string() == "optional");
    REQUIRE_FALSE(item.next());
}

TEST_CASE("write message field") {
    std::string buffer;
    protozero::pbf_writer pbf_test{buffer};

    SECTION("string") {
        std::string buffer_submessage;
        protozero::pbf_writer pbf_submessage{buffer_submessage};
        pbf_submessage.add_string(1, "foobar");

        pbf_test.add_message(1, buffer_submessage);
    }

    SECTION("string with subwriter") {
        protozero::pbf_writer pbf_submessage{pbf_test, 1};
        pbf_submessage.add_string(1, "foobar");
    }

    SECTION("string with subwriter with reserved size") {
        const std::string str{"foobar"};
        const auto size = 1 /* tag */ + 1 /* length field */ + str.size();
        protozero::pbf_writer pbf_submessage{pbf_test, 1, size};
        pbf_submessage.add_string(1, "foobar");
    }

    REQUIRE(buffer == load_data("message/data-message"));
}

TEST_CASE("write message field into non-empty buffer") {
    std::string buffer{"some data already in here"};
    protozero::pbf_writer pbf_test{buffer};

    SECTION("string") {
        std::string buffer_submessage;
        protozero::pbf_writer pbf_submessage{buffer_submessage};
        pbf_submessage.add_string(1, "foobar");

        pbf_test.add_message(1, buffer_submessage);
    }

    SECTION("string with subwriter") {
        protozero::pbf_writer pbf_submessage{pbf_test, 1};
        pbf_submessage.add_string(1, "foobar");
    }

    REQUIRE(buffer == std::string{"some data already in here"} + load_data("message/data-message"));
}

TEST_CASE("write message field reserving memory beforehand") {
    std::string buffer;
    protozero::pbf_writer pbf_test{buffer};
    pbf_test.reserve(100);
    REQUIRE(buffer.capacity() >= 100);

    SECTION("string") {
        std::string buffer_submessage;
        protozero::pbf_writer pbf_submessage{buffer_submessage};
        pbf_submessage.add_string(1, "foobar");

        pbf_test.add_message(1, buffer_submessage);
    }

    SECTION("string with subwriter") {
        protozero::pbf_writer pbf_submessage{pbf_test, 1};
        pbf_submessage.add_string(1, "foobar");
    }

    REQUIRE(buffer == load_data("message/data-message"));
}

TEST_CASE("write optional message field") {
    std::string buffer;
    protozero::pbf_writer pbf_opt{buffer};

    SECTION("add nothing") {
        REQUIRE(buffer == load_data("message/data-opt-empty"));
    }

    SECTION("add string") {
        pbf_opt.add_string(1, "optional");

        REQUIRE(buffer == load_data("message/data-opt-element"));
    }
}


TEST_CASE("move-assigning onto an open subwriter closes its submessage") {
    std::string buffer;
    protozero::pbf_writer pbf_test{buffer};

    protozero::pbf_writer pbf_submessage{pbf_test, 1};
    pbf_submessage.add_string(1, "foobar");

    SECTION("move-assign default constructed writer") {
        pbf_submessage = protozero::pbf_writer{};
        REQUIRE_FALSE(pbf_submessage.valid());
        REQUIRE(buffer == load_data("message/data-message"));
    }

    SECTION("move-assign subwriter of another parent") {
        std::string other_buffer;
        protozero::pbf_writer other_pbf{other_buffer};
        protozero::pbf_writer other_submessage{other_pbf, 1};
        other_submessage.add_string(1, "foobar");

        pbf_submessage = std::move(other_submessage);
        REQUIRE(pbf_submessage.valid());
        REQUIRE(buffer == load_data("message/data-message"));

        pbf_submessage.commit();
        REQUIRE(other_buffer == load_data("message/data-message"));
    }

    // The parent must be usable again after the submessage was closed.
    REQUIRE_NOTHROW(pbf_test.add_string(2, "x"));
}

TEST_CASE("self-move-assigning an open subwriter keeps it open") {
    std::string buffer;
    protozero::pbf_writer pbf_test{buffer};

    protozero::pbf_writer pbf_submessage{pbf_test, 1};
    auto& alias = pbf_submessage;
    pbf_submessage = std::move(alias);
    REQUIRE(pbf_submessage.valid());

    pbf_submessage.add_string(1, "foobar");
    pbf_submessage.commit();
    REQUIRE(buffer == load_data("message/data-message"));
}
