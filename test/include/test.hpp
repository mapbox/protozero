#ifndef TEST_HPP
#define TEST_HPP

#include <catch.hpp>

#include <array>

#include <stdexcept>
// Define protozero_assert() to throw this error. This allows the tests to
// check that the assert fails.
struct assert_error : public std::runtime_error {
    explicit assert_error(const char* what_arg) : std::runtime_error(what_arg) {
    }
};
#define protozero_assert(x) if (!(x)) { throw assert_error{#x}; }

#include <protozero/fixed_size_buffer_adaptor.hpp>
#include <protozero/pbf_builder.hpp>
#include <protozero/pbf_message.hpp>
#include <protozero/pbf_reader.hpp>
#include <protozero/pbf_writer.hpp>

extern std::string load_data(const std::string& filename);

#define PROTOZERO_TEST_CONCAT2(x, y) x##y
#define PROTOZERO_TEST_CONCAT(x, y) PROTOZERO_TEST_CONCAT2(x, y)

#define PROTOZERO_TEST_STRING2(s) #s
#define PROTOZERO_TEST_STRING(s) PROTOZERO_TEST_STRING2(s)

// The following structs are used in many tests using TEMPLATE_TEST_CASE()
// to test a dynamically sized buffer based on std::string and a statically
// sized buffer based on std::array.

struct test_type_dynamic_buffer {
    using type = std::string;
    using writer_type = protozero::pbf_writer; // == protozero::basic_pbf_writer<type>;

    std::string m_buffer;

    type& buffer() noexcept {
        return m_buffer;
    }

    const char *data() const noexcept {
        return m_buffer.data();
    }

    std::size_t size() const noexcept {
        return m_buffer.size();
    }
};

struct test_type_static_buffer {
    using type = protozero::fixed_size_buffer_adaptor;
    using writer_type = protozero::basic_pbf_writer<type>;

    std::array<char, 1024> m_buffer = {{0}};
    type adaptor{m_buffer.data(), m_buffer.size()};

    type& buffer() noexcept {
        return adaptor;
    }

    const char *data() const noexcept {
        return adaptor.data();
    }

    std::size_t size() const noexcept {
        return adaptor.size();
    }
};
#endif // TEST_HPP
