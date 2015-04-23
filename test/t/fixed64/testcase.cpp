
#include <cassert>
#include <fstream>

#include "testcase.pb.h"

int main(int c, char *argv[]) {
    TestFixed64::Test msg;
    msg.set_i(1ll << 40);

    std::string out;
    msg.SerializeToString(&out);

    {
        std::ofstream d("data1.bin");
        assert(d.is_open());
        d << out;
    }

    msg.set_i(-(1ll << 42));
    msg.SerializeToString(&out);

    {
        std::ofstream d("data2.bin");
        assert(d.is_open());
        d << out;
    }
}

