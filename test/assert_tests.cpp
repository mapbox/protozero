
// Tests for protozero_assert() checks in noexcept functions.
//
// The other tests define protozero_assert() to throw (see test.hpp), so that
// they can check for failed asserts with REQUIRE_THROWS_AS. That doesn't work
// in noexcept functions, because the exception calls std::terminate(). Here
// protozero_assert() ends the process instead, so every test case needs a
// process of its own. test/CMakeLists.txt adds a ctest test for each of them;
// when adding a test case here, add its name there, too. Tests that check
// that something does *not* assert belong with the other tests.
//
// A test case sets expected_assert to the assert message (with quotes) it
// expects. The process exits successfully only if an assert fails and its
// expression ends with that message. If no assert fails, the FAIL() at the
// end of the test case makes the process exit with an error. Matching the
// whole message matters: FAIL() throws, and destructors running during stack
// unwinding can fail other asserts, which must not count as success.

#include <catch.hpp>

#include <cstdio>
#include <cstdlib>
#include <string>
#include <utility>

namespace {

// The message (including quotes) the asserts checked by the current test case
// end with: protozero_assert(condition && "message").
std::string expected_assert;

bool ends_with(const std::string& str, const std::string& suffix) {
    return str.size() >= suffix.size() &&
           str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

[[noreturn]] void assert_failed(const char* expr) {
    std::printf("assert failed: %s\n", expr);
    const bool ok = !expected_assert.empty() && ends_with(expr, expected_assert);
    // Don't use std::exit(), it would run the destructors of Catch's static
    // objects in the middle of a test case.
    std::fflush(stdout);
    std::_Exit(ok ? EXIT_SUCCESS : EXIT_FAILURE);
}

const char* const open_submessage = R"("you can't move a basic_pbf_writer with an open submessage")";

} // anonymous namespace

#define protozero_assert(x) if (!(x)) { assert_failed(#x); } // NOLINT(readability-simplify-boolean-expr)

#include <protozero/pbf_writer.hpp>

// Moving or swapping a writer with an open submessage leaves the sub-writer
// pointing to the moved-from writer, so these must assert.

TEST_CASE("move construct parent with open child") {
    expected_assert = open_submessage;
    std::string buffer;
    protozero::pbf_writer pw{buffer};
    const protozero::pbf_writer sub{pw, 1};
    const protozero::pbf_writer pw2{std::move(pw)};
    FAIL("expected assert did not happen");
}

TEST_CASE("move assign from parent with open child") {
    expected_assert = open_submessage;
    std::string buffer;
    protozero::pbf_writer pw{buffer};
    const protozero::pbf_writer sub{pw, 1};
    protozero::pbf_writer pw2;
    pw2 = std::move(pw);
    FAIL("expected assert did not happen");
}

TEST_CASE("move assign onto parent with open child") {
    expected_assert = open_submessage;
    std::string buffer;
    protozero::pbf_writer pw{buffer};
    const protozero::pbf_writer sub{pw, 1};
    pw = protozero::pbf_writer{};
    FAIL("expected assert did not happen");
}

TEST_CASE("swap parent with open child") {
    expected_assert = open_submessage;
    std::string buffer;
    protozero::pbf_writer pw{buffer};
    const protozero::pbf_writer sub{pw, 1};
    protozero::pbf_writer pw2;
    pw2.swap(pw);
    FAIL("expected assert did not happen");
}
