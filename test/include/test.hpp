
#include <catch.hpp>

#include <stdexcept>
// Define protozero_assert() to throw this error. This allows the tests to
// check that the assert fails.
struct assert_error : public std::runtime_error {
    assert_error(const char* what_arg) : std::runtime_error(what_arg) {
    }
};
#define protozero_assert(x) if (!(x)) { throw(assert_error(#x)); }

#include <protozero/pbf_reader.hpp>
#include <protozero/pbf_writer.hpp>

extern std::string load_data(const std::string& filename);

