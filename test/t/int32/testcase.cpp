
#include <testcase.hpp>
#include "testcase.pb.h"

int main(int c, char *argv[]) {
    TestInt32::Test msg;

    msg.set_i(0);
    write_to_file(msg, "data-zero.bin");

    msg.set_i(1);
    write_to_file(msg, "data-pos.bin");

    msg.set_i(-1);
    write_to_file(msg, "data-neg.bin");

    msg.set_i(std::numeric_limits<int32_t>::max());
    write_to_file(msg, "data-max.bin");

    msg.set_i(std::numeric_limits<int32_t>::min());
    write_to_file(msg, "data-min.bin");
}

