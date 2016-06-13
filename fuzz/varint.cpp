#include <cassert>
#include <cstdint>
#include <cctype>

#include <string>
#include <iterator>
#include <stdexcept>

#include <protozero/varint.hpp>

extern "C" int LLVMFuzzerTestOneInput(const unsigned char *data, unsigned long size) {
  /* Discard non-integral input */

  if (!std::all_of(data, data + size, ::isdigit))
    return 0;

  const std::string in(reinterpret_cast<const char *>(data), size);

  /* Swallow exceptions from std::stol(l) in case of overflows */

  /* TODO(daniel-j-h): SFINAE dispatch size for stoi, stol, stoll */

  try {
    const std::int32_t num32 = stoi(in);
    assert(protozero::decode_zigzag32(protozero::encode_zigzag32(num32)) == num32);
  } catch (const std::invalid_argument&) {
  } catch (const std::out_of_range&) {
  }

  try {
    const std::int64_t num64 = stol(in);
    assert(protozero::decode_zigzag64(protozero::encode_zigzag64(num64)) == num64);
  } catch (const std::invalid_argument&) {
  } catch (const std::out_of_range&) {
  }

  return 0; /* Always return zero, sanitizers hard-abort */

}
