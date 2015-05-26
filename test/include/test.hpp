
#include <catch.hpp>

#include <stdexcept>
// Define pbf_assert() to throw this error. This allows the tests to check that
// the assert fails.
struct assert_error : public std::runtime_error {
    assert_error(const char* what_arg) : std::runtime_error(what_arg) {
    }
};
#define pbf_assert(x) if (!(x)) { throw(assert_error(#x)); }

#include <pbf_reader.hpp>
#include <pbf_writer.hpp>

extern std::string get_file_data(const std::string& filename);

