# pbf.hpp

Minimalistic fast C++ decoder for a subset of the protocol buffer format.

This is header-only, meaning there is nothing to build. Just include `pbf.hpp`
in your C++ application.

Low-level: this is designed to be a building block for writing a very
customized, lazy decoder for a stable protobuf schema. If your protobuf schema
is changing frequently or lazy decoding is not critical for your application
then this approach offers no value: just use the decoding API available via the
C++ API that can be generated via the Google Protobufs `protoc` program.

[![Build Status](https://travis-ci.org/mapbox/pbf.hpp.svg?branch=master)](https://travis-ci.org/mapbox/pbf.hpp)
[![Coverage Status](https://coveralls.io/repos/mapbox/pbf.hpp/badge.svg?branch=master)](https://coveralls.io/r/mapbox/pbf.hpp?branch=master)


## Depends

 - C++11 compiler
 - A working knowledge of how
   [protocol buffer encoding works](https://developers.google.com/protocol-buffers/docs/encoding).


## Limits

* The current implementation does not support big-endian machines. Fixed sized
  integers and floats/doubles will not decode properly.


## Tests

Minimal right now. To see how to extract the layer names from [a vector tile](https://github.com/mapbox/vector-tile-spec) run:

    make clean && make test

