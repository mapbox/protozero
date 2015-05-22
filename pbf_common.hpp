#ifndef MAPBOX_UTIL_PBF_COMMON_HPP
#define MAPBOX_UTIL_PBF_COMMON_HPP

#include <cstdint>

namespace mapbox { namespace util {

    typedef uint32_t pbf_tag_type;

    enum class pbf_wire_type : uint32_t {
        varint           = 0,
        fixed64          = 1,
        length_delimited = 2,
        fixed32          = 5,
        unknown          = 99
    };

}} // end namespace mapbox::util

#endif // MAPBOX_UTIL_PBF_COMMON_HPP
