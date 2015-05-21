
#include <catch.hpp>

#include <stdexcept>
// Define assert() to throw this error. This enables the tests to check that
// the assert() fails.
struct assert_error : public std::runtime_error {
    assert_error(const char* what_arg) : std::runtime_error(what_arg) {
    }
};
#undef assert
#define assert(x) if (!(x)) { throw(assert_error(#x)); }

#include <pbf.hpp>
#include <pbf_writer.hpp>

extern std::string get_file_data(const std::string& filename);

