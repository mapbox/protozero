#ifndef BUFFER_HPP
#define BUFFER_HPP

#include "test.hpp"

// This "simulates" an externally defined buffer type to make sure our
// buffer adaptor functions do the right thing.
namespace test_external {

    class ext_buffer : public std::string {
    };

    inline std::size_t buffer_size(const ext_buffer* buffer) noexcept {
        return buffer->size();
    }

    inline void buffer_append(ext_buffer* buffer, const char* data, std::size_t count) {
        buffer->append(data, count);
    }

    inline void buffer_append_zeros(ext_buffer* buffer, std::size_t count) {
        buffer->append(count, '\0');
    }

    inline void buffer_resize(ext_buffer* buffer, std::size_t size) {
        protozero_assert(size < buffer_size(buffer));
        buffer->resize(size);
    }

    inline void buffer_reserve_additional(ext_buffer* buffer, std::size_t size) {
        buffer->reserve(buffer->size() + size);
    }

    inline void buffer_erase_range(ext_buffer* buffer, std::size_t from, std::size_t to) {
        protozero_assert(from <= buffer->size());
        protozero_assert(to <= buffer->size());
        protozero_assert(from <= to);
        buffer->erase(std::next(buffer->begin(), from), std::next(buffer->begin(), to));
    }

    inline char* buffer_at_pos(ext_buffer* buffer, std::size_t pos) {
        protozero_assert(pos <= buffer->size());
        return (&*buffer->begin()) + pos;
    }

    inline void buffer_push_back(ext_buffer* buffer, char ch) {
        buffer->push_back(ch);
    }
}; // namespace test_external

// The following structs are used in many tests using TEMPLATE_TEST_CASE() to
// test the different buffer types:
//
// 1. Dynamically sized buffer based on std::string.
// 2. Dynamically sized buffer based on std::vector<char>.
// 3. Statically sized buffer based on std::array<char, N>.
// 4. Externally defined buffer.

class buffer_test_string {

    std::string m_buffer;

public:

    using type = std::string;
    using writer_type = protozero::pbf_writer; // == protozero::basic_pbf_writer<type>;

    type& buffer() noexcept {
        return m_buffer;
    }

    const char *data() const noexcept {
        return m_buffer.data();
    }

    std::size_t size() const noexcept {
        return m_buffer.size();
    }
}; // class buffer_test_string

class buffer_test_vector {

    std::vector<char> m_buffer;

public:

    using type = std::vector<char>;
    using writer_type = protozero::basic_pbf_writer<type>;

    type& buffer() noexcept {
        return m_buffer;
    }

    const char *data() const noexcept {
        return m_buffer.data();
    }

    std::size_t size() const noexcept {
        return m_buffer.size();
    }
}; // class buffer_test_vector

class buffer_test_array {

public:
    using type = protozero::fixed_size_buffer_adaptor;
    using writer_type = protozero::basic_pbf_writer<type>;

    type& buffer() noexcept {
        return adaptor;
    }

    const char *data() const noexcept {
        return adaptor.data();
    }

    std::size_t size() const noexcept {
        return adaptor.size();
    }

private:

    std::array<char, 1024> m_buffer = {{0}};
    type adaptor{m_buffer.data(), m_buffer.size()};

}; // class buffer_test_array

class buffer_test_external {

    test_external::ext_buffer m_buffer;

public:

    using type = test_external::ext_buffer;
    using writer_type = protozero::basic_pbf_writer<type>;

    type& buffer() noexcept {
        return m_buffer;
    }

    const char *data() const noexcept {
        return m_buffer.data();
    }

    std::size_t size() const noexcept {
        return m_buffer.size();
    }
}; // class buffer_test_external

#endif // BUFFER_HPP
