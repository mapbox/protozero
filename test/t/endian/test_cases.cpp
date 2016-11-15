
#include <cstdint>

#include <test.hpp>

#include <protozero/byteswap.hpp>

namespace {

    int32_t check_swap_4(int32_t data) {
        protozero::detail::byteswap_inplace(data);
        protozero::detail::byteswap_inplace(data);
        return data;
    }

    int64_t check_swap_8(int64_t data) {
        protozero::detail::byteswap_inplace(data);
        protozero::detail::byteswap_inplace(data);
        return data;
    }

}

TEST_CASE("byte swapping") {
    REQUIRE(0 == check_swap_4(0));
    REQUIRE(1 == check_swap_4(1));
    REQUIRE(-1 == check_swap_4(-1));
    REQUIRE(395503 == check_swap_4(395503));
    REQUIRE(-804022 == check_swap_4(-804022));

    REQUIRE(0 == check_swap_8(0));
    REQUIRE(1 == check_swap_8(1));
    REQUIRE(-1 == check_swap_8(-1));
    REQUIRE(395503 == check_swap_8(395503));
    REQUIRE(-804022 == check_swap_8(-804022));
    REQUIRE(3280329805 == check_swap_8(3280329805));
    REQUIRE(-2489204041 == check_swap_8(-2489204041));
}

