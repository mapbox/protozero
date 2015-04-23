
#include "../../testcase.hpp"
#include "testcase.pb.h"

int main(int c, char *argv[]) {
    TestFixed32::Test msg;

    msg.set_i(999);
    write_to_file(msg, "data1.bin");

    msg.set_i(-189);
    write_to_file(msg, "data2.bin");
}

