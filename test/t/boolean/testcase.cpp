
#include <cassert>
#include <fstream>

#include "testcase.pb.h"

int main(int c, char *argv[]) {
    TestBoolean::Test msg;
    msg.set_i(0);

    std::string out;
    msg.SerializeToString(&out);

    {
        std::ofstream d("data-false.bin");
        assert(d.is_open());
        d << out;
    }

    msg.set_i(1);
    msg.SerializeToString(&out);

    {
        std::ofstream d("data-true.bin");
        assert(d.is_open());
        d << out;
    }

    msg.set_i(2);
    msg.SerializeToString(&out);

    {
        std::ofstream d("data-also-true.bin");
        assert(d.is_open());
        d << out;
    }

    msg.set_i(127);
    msg.SerializeToString(&out);

    {
        std::ofstream d("data-still-true.bin");
        assert(d.is_open());
        d << out;
    }
}

