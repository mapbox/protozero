
#include <cassert>
#include <fstream>

#include "testcase.pb.h"

int main(int c, char *argv[]) {
    TestDouble::Test msg;
    msg.set_x(4.893);

    std::string out;
    msg.SerializeToString(&out);

    {
        std::ofstream d("data1.bin");
        assert(d.is_open());
        d << out;
    }

    msg.set_x(-9232.33);
    msg.SerializeToString(&out);

    {
        std::ofstream d("data2.bin");
        assert(d.is_open());
        d << out;
    }
}

