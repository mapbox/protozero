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


## How it works

Pbf.hpp does **not** read `.proto` files used by the usual Protobuf
implementations. The developer using `pbf.hpp` has to manually "translate" the
`.proto` description into code. This means there is no way to access any of the
information from the `.proto` description. This results in a few restrictions:

* The names of the fields are not available.
* Enum names are not available, you'll have to use the values they are defined
  with.
* Default values are not available.
* Field types have to be hardcoded. The library does not know which types to
  expect, so the user of the library has to supply the right types. Some checks
  are made using `assert()`, but mostly the user has to take care of that.
* The length of a string, bytes, or submessage can't be more than 2^31-1.

The library will make sure not to overrun the buffer it was given, but
basically all other checks have to be made in user code!

Your code will basically always look like this:

    // get data from somewhere
    // initialize pbf message with this data
    mapbox::util::pbf message(input.data(), input.size());

    // iterate over fields in the message
    while (message.next()) {
        // switch depending on the tag (because the name is not available)
        switch (message.tag) {
            case 1:
                // get data for tag 1 (in this case a varint)
                uint32_t x = message.varint();
                break;
            case 2:
                // get data for tag 2 (in this case a string)
                std::string s = message.string();
                break;
            default:
                // error: unknown tag / or ignore
        }
    }


## Limitations

* The current implementation does not support big-endian machines. Fixed sized
  integers and floats/doubles will not decode properly.
* A protobuf message has to fit into memory completely, otherwise it can not
  be parsed with this library. There is no streaming support.
* Default values are not available.
* The Google Protobuf spec documents that a non-repeated field can actually
  appear several times in a message and the implementation is required to
  return the value of the last version of that field in this case. `pbf.hpp`
  does not enforce this. If this feature is needed in your case, you have to
  do this yourself.


## Tests

Minimal right now. To see how to extract the layer names from [a vector tile](https://github.com/mapbox/vector-tile-spec) run:

    make clean && make test

