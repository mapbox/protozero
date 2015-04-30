
#include <testcase.hpp>
#include "testcase.pb.h"

int main(int c, char *argv[]) {
    TestString::Test msg;

    msg.set_s("");
    write_to_file(msg, "data-empty.bin");

    msg.set_s("x");
    write_to_file(msg, "data-one.bin");

    msg.set_s("foobar");
    write_to_file(msg, "data-string.bin");
}

