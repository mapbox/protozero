
# Protozero Advanced Topics

This documentation contains some mixed advanced topics for Protozero users.
Read the [tutorial](tutorial.md) first if you are new to Protozero.


## Limitations of Protozero

* A protobuf message has to fit into memory completely, otherwise it can not
  be parsed with this library. There is no streaming support.
* The length of a string, bytes, or submessage can't be more than 2^31-1.
* There is no specific support for maps but they can be used as described in
  the "Backwards compatibility" section of
  https://developers.google.com/protocol-buffers/docs/proto3#maps.


## Checking the Protozero version number

If `protozero/version.hpp` is included, the following macros are set:

| Macro                      | Example | Description                                    |
| -------------------------- | ------- | ---------------------------------------------- |
| `PROTOZERO_VERSION_MAJOR`  | 1       | Major version number                           |
| `PROTOZERO_VERSION_MINOR`  | 3       | Minor version number                           |
| `PROTOZERO_VERSION_PATCH`  | 2       | Patch number                                   |
| `PROTOZERO_VERSION_CODE`   | 10302   | Version (major * 10,000 + minor * 100 + patch) |
| `PROTOZERO_VERSION_STRING` | "1.3.2" | Version string                                 |


## Changing Protozero behaviour with macros

The behaviour of Protozero can be changed by defining the following macros.
They have to be set before including any of the Protozero headers.

## `PROTOZERO_STRICT_API`

If this is set, you will get some extra warnings or errors during compilation
if you are using an old (deprecated) interface to Protozero. Enable this if
you want to make sure your code will work with future versions of Protozero.

## `PROTOZERO_USE_VIEW`

Protozero uses the class `protozero::data_view` as the return type of the
`pbf_reader::get_view()` method and a few other functions take a
`protozero::data_view` as parameter.

If `PROTOZERO_USE_VIEW` is unset, `protozero::data_view` is Protozero's own
implementation of a *string view* class.

Set this macro if you want to use a different implementation such as the C++17
`std::string_view` class. In this case `protozero::data_view` will simply be
an alias to the class you specify.

    #define PROTOZERO_USE_VIEW std::string_view


## Repeated fields in messages

The Google Protobuf spec documents that a non-repeated field can actually
appear several times in a message and the implementation is required to return
the value of the last version of that field in this case. `pbf_reader.hpp` does
not enforce this. If this feature is needed in your case, you have to do this
yourself.

The [spec also
says](https://developers.google.com/protocol-buffers/docs/encoding#packed)
that you must be able to read a packed repeated field where a not-packed
repeated field is expected and vice versa. Also there can be several (packed or
not-packed) repeated fields with the same tag and their contents must be
concatenated. It is your responsibility to do this, Protozero doesn't do that
for you.


## Reserving memory when writing messages

If you know beforehand how large a message will become or can take an educated
guess, you can call the usual `std::string::reserve()` on the underlying string
before you give it to an `pbf_writer` or `pbf_builder` object.

Or you can (at any time) call `reserve()` on the `pbf_writer` or `pbf_builder`.
This will reserve the given amount of bytes *in addition to whatever is already
in that message*. (Note that this behaviour is different then what `reserve()`
does on `std::string` or `std::vector`.)

In the general case it is not easy to figure out how much memory you will need
because of the varint packing of integers. But sometimes you can make at least
a rough estimate. Still, you should probably only use this facility if you have
benchmarks proving that it actually makes your program faster.


## Using the Low-Level Varint and Zigzag Encoding and Decoding Functions

Protozero gives you access to the low-level functions for encoding and
decoding varint and zigzag integer encodings, because these functions can
sometimes be useful outside the Protocol Buffer context.

### Using Low-Level Functions

To use the low-level functions, add this include to your C++ program:

```cpp
#include <protozero/varint.hpp>
```

### Functions

The following functions are then available:

```cpp
decode_varint()
write_varint()
encode_zigzag32()
encode_zigzag64()
decode_zigzag32()
decode_zigzag64()
```

See the reference documentation created by `make doc` for details.

