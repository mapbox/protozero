
#include "../../testcase.hpp"
#include "testcase.pb.h"

int main(int c, char *argv[]) {
    TestFloat::Test msg;

    msg.set_x(5.34);
    write_to_file(msg, "data1.bin");

    msg.set_x(-1.71);
    write_to_file(msg, "data2.bin");
}

