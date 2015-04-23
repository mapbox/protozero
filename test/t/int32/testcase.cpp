
#include "../../testcase.hpp"
#include "testcase.pb.h"

int main(int c, char *argv[]) {
    TestInt32::Test msg;

    msg.set_i(17);
    write_to_file(msg, "data1.bin");

    msg.set_i(-1);
    write_to_file(msg, "data2.bin");
}

