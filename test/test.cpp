
#include <vector>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include "../pbf.hpp"


int main() {

    std::string filename("./test/14_8716_8015.vector.pbf");
    std::ifstream stream(filename.c_str(),std::ios_base::in|std::ios_base::binary);
    if (!stream.is_open())
    {
        throw std::runtime_error("could not open: '" + filename + "'");
    }
    std::string buffer(std::istreambuf_iterator<char>(stream.rdbuf()),(std::istreambuf_iterator<char>()));
    stream.close();
    protobuf::message item(buffer.data(),buffer.size());
    std::clog << "layer names:\n";
    while (item.next()) {
        // it's a layer according to
        // https://github.com/mapbox/mapnik-vector-tile/blob/master/proto/vector_tile.proto
        if (item.tag == 3) {
            uint64_t len = item.varint();
            protobuf::message layermsg(item.getData(),static_cast<std::size_t>(len));
            while (layermsg.next()) {
                if (layermsg.tag == 1) {
                    std::clog << "\t" << layermsg.string() << "\n";
                } else {
                    layermsg.skip();
                }
            }
            item.skipBytes(len);
        } else {
            item.skip();
        }
    }
}