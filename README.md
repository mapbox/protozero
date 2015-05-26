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

The library will make sure not to overrun the buffer it was given, but
basically all other checks have to be made in user code!

See the [tutorial](tutorial.md) for more information on how to use it.

Call `make doc` to build the Doxygen documentation. (You'll need
[Doxygen](http://www.stack.nl/~dimitri/doxygen/) installed.) Then open
`doc/html/index.html` in your browser to read it.


## Limitations

* The current implementation does not support big-endian machines. Fixed sized
  integers and floats/doubles will not decode properly.
* A protobuf message has to fit into memory completely, otherwise it can not
  be parsed with this library. There is no streaming support.
* The length of a string, bytes, or submessage can't be more than 2^31-1.
* The Google Protobuf spec documents that a non-repeated field can actually
  appear several times in a message and the implementation is required to
  return the value of the last version of that field in this case.
  `pbf_reader.hpp` does not enforce this. If this feature is needed in your
  case, you have to do this yourself.


## Tests

Extensive tests are included. Call

    make test

to build all tests and run them.

With Clang you can get a test coverage report with

    CXXFLAGS="--coverage" LDFLAGS="--coverage" make test

You can also use `gyp` to build the tests:

    gyp --depth=. --build=Release
    ./out/Release/tests

This will clobber the `Makefile` from the repository! Instead of `Release` you
can use `Debug` for a debug build.

## Cppcheck

For extra checks with [Cppcheck](http://cppcheck.sourceforge.net/) you can call

    make check

