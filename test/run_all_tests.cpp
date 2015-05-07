
#include <fstream>
#include <stdexcept>
#include <string>

#define CATCH_CONFIG_MAIN
#include <test.hpp> // IWYU pragma: keep

std::string get_file_data(const std::string& filename) {
    std::ifstream stream(filename.c_str(), std::ios_base::in|std::ios_base::binary);
    if (!stream.is_open())
    {
        throw std::runtime_error("could not open: '" + filename + "'");
    }
    std::string buffer(std::istreambuf_iterator<char>(stream.rdbuf()), (std::istreambuf_iterator<char>()));
    stream.close();

    return buffer;
}

