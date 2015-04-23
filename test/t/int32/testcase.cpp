
#include <cassert>
#include <fstream>

#include "testcase.pb.h"

int main(int c, char *argv[]) {
    TestInt32::Test msg;
    msg.set_i(17);

    std::string out;
    msg.SerializeToString(&out);

    {
        std::ofstream d("data1.bin");
        assert(d.is_open());
        d << out;
    }

    msg.set_i(-1);
    msg.SerializeToString(&out);

    {
        std::ofstream d("data2.bin");
        assert(d.is_open());
        d << out;
    }
}

