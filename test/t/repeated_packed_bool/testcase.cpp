
#include <testcase.hpp>

#include "testcase.pb.h" // NOLINT(clang-diagnostic-error)

int main() {
    TestRepeatedPackedBool::Test msg;

    write_to_file(msg, "data-empty.pbf");

    msg.add_b(true);
    write_to_file(msg, "data-one.pbf");

    msg.add_b(true);
    msg.add_b(false);
    msg.add_b(true);
    write_to_file(msg, "data-many.pbf");
}

