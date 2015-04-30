
#include <testcase.hpp>
#include "testcase.pb.h"

int main(int c, char *argv[]) {
    TestBoolean::Test msg;

    msg.set_b(0);
    write_to_file(msg, "data-false.bin");

    msg.set_b(1);
    write_to_file(msg, "data-true.bin");

    msg.set_b(2);
    write_to_file(msg, "data-also-true.bin");

    msg.set_b(127);
    write_to_file(msg, "data-still-true.bin");
}

