
#include <testcase.hpp>
#include "testcase.pb.h"

int main(int c, char *argv[]) {
    TestMessage::Test msg;

    TestMessage::Sub* submsg = msg.mutable_submessage();
    submsg->set_s("foobar");

    write_to_file(msg, "data-message.pbf");
}

