
#include <testcase.hpp>
#include "testcase.pb.h"

int main(int c, char *argv[]) {
    TestEnum::Test msg;

    msg.set_color(TestEnum::BLACK);
    write_to_file(msg, "data-black.pbf");

    msg.set_color(TestEnum::BLUE);
    write_to_file(msg, "data-blue.pbf");
}

