
#include "../../testcase.hpp"
#include "testcase.pb.h"

int main(int c, char *argv[]) {
    TestFixed64::Test msg;

    msg.set_i(1ll << 40);
    write_to_file(msg, "data1.bin");

    msg.set_i(-(1ll << 42));
    write_to_file(msg, "data2.bin");
}

