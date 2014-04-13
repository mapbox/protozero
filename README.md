# pbf.hpp

A protocol buffer decoding implementation in C++.

This is header-only, meaning there is nothing to build. Just include it in your C++ application.

Low-level: this is designed to be a building block for writing a very customized, lazy decoder for a stable protobuf schema. If your protobuf schema is changing frequently or lazy decoding is not critical for your application then this approach offers no value: just use the decoding API available via the C++ API that can be generated via the Google Protobufs `protoc` program.

## Depends

 - C++ compiler
 - A working knowledge of how [protocol buffer encoding works](https://developers.google.com/protocol-buffers/docs/encoding).


## Tests

Minimal right now. To see how to extract the layer names from [a vector tile](https://github.com/mapbox/vector-tile-spec) run:

    make clean && make test