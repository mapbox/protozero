
#include <cassert>
#include <fstream>

#include "testcase.pb.h"

int main(int c, char *argv[]) {
    TestString::Test msg;
    msg.set_s("foobar");

    std::string out;
    msg.SerializeToString(&out);

    {
        std::ofstream d("data1.bin");
        assert(d.is_open());
        d << out;
    }

    msg.set_s("");
    msg.SerializeToString(&out);

    {
        std::ofstream d("data2.bin");
        assert(d.is_open());
        d << out;
    }
}

