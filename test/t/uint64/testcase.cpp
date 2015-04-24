
#include "../../testcase.hpp"
#include "testcase.pb.h"

int main(int c, char *argv[]) {
    TestUInt64::Test msg;

    msg.set_i(0ul);
    write_to_file(msg, "data-zero.bin");

    msg.set_i(1);
    write_to_file(msg, "data-pos.bin");

    msg.set_i(std::numeric_limits<uint64_t>::max());
    write_to_file(msg, "data-max.bin");
}

