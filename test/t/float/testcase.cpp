
#include <cassert>
#include <fstream>

#include "testcase.pb.h"

int main(int c, char *argv[]) {
    TestFloat::Test msg;
    msg.set_x(5.34);

    std::string out;
    msg.SerializeToString(&out);

    {
        std::ofstream d("data1.bin");
        assert(d.is_open());
        d << out;
    }

    msg.set_x(-1.71);
    msg.SerializeToString(&out);

    {
        std::ofstream d("data2.bin");
        assert(d.is_open());
        d << out;
    }
}

