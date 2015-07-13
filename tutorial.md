
# Protozero Tutorial

## Getting to Know Protocol Buffers

Protozero is a very low level library. You really have to know some of the
insides of Protocol Buffers to work with it!

So before reading any further in this document, read the following from the
Protocol Buffer documentation:

* [Developer Guide - Overview](https://developers.google.com/protocol-buffers/docs/overview)
* [Language Guide](https://developers.google.com/protocol-buffers/docs/proto)
* [Encoding](https://developers.google.com/protocol-buffers/docs/encoding)

Make sure you understand the basic types of values supported by Protocol
Buffers. Refer to this
[handy table](https://developers.google.com/protocol-buffers/docs/proto#scalar)
if you are getting lost.


## Prerequisites

You need a C++11-capable compiler for protozero to work. Copy the files in the
`include` directory somewhere where your build system can find them. You always
need `protozero/pbf_common.hpp`, for reading and writing support you need
`protozero/pbf_reader.hpp` and `protozero/pbf_writer.hpp`, respectively.


## Parsing protobuf-encoded messages

### Using `pbf_reader.hpp`

All you have to do to use `pbf_reader.hpp` is to include it in your C++
program:

    #include <protozero/pbf_reader.hpp>

`pbf_reader.hpp` contains asserts that will detect some programming errors. We
encourage you to compile with asserts enabled in your debug builds.


### An Introductory Example

Lets say you have a protocol description in a `.proto` file like this:

    message Example1 {
        required uint32 x = 1;
        optional string s = 2;
        repeated fixed64 r = 17;
    }

To read messages created according to that description, you will have code that
looks somewhat like this:

    #include <protozero/pbf_reader.hpp>

    // get data from somewhere into the input string
    std::string input = get_input_data();

    // initialize pbf message with this data
    protozero::pbf message(input.data(), input.size());

    // iterate over fields in the message
    while (message.next()) {

        // switch depending on the field tag (the field name is not available)
        switch (message.tag()) {
            case 1:
                // get data for tag 1 (in this case an uint32)
                auto x = message.get_uint32();
                break;
            case 2:
                // get data for tag 2 (in this case a string)
                std::string s = message.get_string();
                break;
            case 17:
                // ignore data for tag 17
                message.skip();
                break;
            default:
                // ignore data for unknown tags to allow for future extensions
                message.skip();
        }

    }

You always have to call `next()` and then either one of the accessor functions
(like `get_uint32()` or `get_string()`) to get the field value or `skip()` to
ignore this field. Then call `next()` again, and so forth. Never call `next()`
twice in a row or any if the accessor or skip functions twice in a row.

Because the `pbf` class doesn't know the `.proto` file it doesn't know which
field names or tags there are and it doesn't known the types of the fields.
You have to make sure to call the right `get_...()` function for each tag. Some
`assert()s` are done to check you are calling the right functions, but not all
errors can be detected.

Note that it doesn't matter whether a field is defined as `required`,
`optional`, or `repeated`. You always have to be prepared to get zero, one, or
more instances of a field and you always have to be prepared to get other
fields, too, unless you want your program to break if somebody adds a new
field.


### If You Only Need a Single Field

If, out of a protocol buffer message, you only need the value of a single
field, you can use the version of the `next()` function with a parameter:

    // same .proto file and initialization as above

    // get all fields with tag 17, skip all others
    while (message.next(17)) {
        auto r = message.get_fixed64();
        std::cout << r << "\n";
    }


### Handling Scalar Fields

As you saw in the example, handling scalar field types is reasonably easy. You
just check the `.proto` file for the type of a field and call the corresponding
function called `get_` + _field type_.

For `string` and `bytes` types the internal handling is exactly the same, but
both `get_string()` and `get_bytes()` are provided to make the code
self-documenting. Both theses calls allocate and return a `std::string` which
can add some overhead. You can call the `get_data()` function instead which
returns a `std::pair<const char*, uint32_t>`, ie a pointer into the data and
the length of the data.


### Handling Repeated Packed Fields

Fields that are marked as `[packed=true]` in the `.proto` file are handled
somewhat differently. `get_packed_...()` functions returning an iterator pair
are used to access the data.

So, for example, if you have a protocol description in a `.proto` file like
this:

    message Example2 {
        repeated sint32 i = 1 [packed=true];
    }

You can get to the data like this:

    protozero::pbf message(input.data(), input.size());

    // set current field
    item.next();

    // get an iterator pair
    auto pi = item.get_packed_sint32();

    // iterate to get to all values
    for (auto it = pi.first; it != pi.second; ++it) {
        std::cout << *it << "\n";
    }

So you are getting a pair of normal forward iterators that can be used with any
STL algorithm etc.

Note that the previous only applies to repeated **packed** fields, normal
repeated fields are handled in the usual way for scalar fields.


### Handling Embedded Messages

Protocol Buffers can embed any message inside another message. To access an
embedded message use the `get_message()` function. So for this description:

    message Point {
        required double x = 1;
        required double y = 2;
    }

    message Example3 {
        repeated Point point = 10;
    }

you can parse with this code:

    protozero::pbf message(input.data(), input.size());

    while (message.next(10)) {
        protozero::pbf point = message.get_message();
        double x, y;
        while (point.next()) {
            switch (point.tag()) {
                case 1:
                    x = point.get_double();
                    break;
                case 2:
                    y = point.get_double();
                    break;
                default:
                    point.skip();
            }
        }
        std::cout << "x=" << x << " y=" << y << "\n";
    }


### Handling Enums

Enums are stored as varints and they can't be differentiated from them. Use
the `get_enum()` function to get the value of the enum, you have to translate
this into the symbolic name yourself. See the `enum` test case for an example.


### Exceptions

All exceptions thrown by `pbf_reader.hpp` functions derive from
`protozero::pbf::exception`.

Note that all exceptions can also happen if you are expecting a data field of
a certain type in your code but the field actually has a different type. In
that case the `pbf` class might interpret the bytes in the buffer in the wrong
way and anything can happen.

#### `end_of_buffer_exception`

This will be thrown whenever any of the functions "runs out of input data".
It means you either have an incomplete message in your input or some other
data corruption has taken place.

#### `unknown_field_type_exception`

This will be thrown if an unsupported field type is encountered. Either your
input data is corrupted or it was written with an unsupported version of a
Protocol Buffers implementation.

#### `varint_too_long_exception`

This exception indicates an illegal encoding of a varint. It means your input
data is corrupted in some way.


### The `pbf` Class

The `pbf` class behaves like a value type. Objects are reasonably small (two
pointers and two `uint32_t`, so 24 bytes on a 64bit system) and they can be
copied and moved around trivially.

Objects of the `pbf` class store a pointer into the input data that was given
to the constructor. You have to make sure this pointer stays valid for the
duration of the objects lifetime.


## Writing Protobuf-Encoded Messages

### Using `pbf_writer.hpp`

All you have to do to use `pbf_writer.hpp` is to include it in your C++
program:

    #include <pbf_writer.hpp>

`pbf_writer.hpp` contains asserts that will detect some programming errors. We
encourage you to compile with asserts enabled in your debug builds.

### An Introductory Example

Lets say you have a protocol description in a `.proto` file like this:

    message Example1 {
        required uint32 x = 1;
        optional string s = 2;
        repeated fixed64 r = 17;
    }

To write messages created according to that description, you will have code
that looks somewhat like this:

    #include <pbf_writer.hpp>

    std::string data;
    osmium::util::pbf_writer pw(data);

    pw.add_uint32(1, 27);       // uint32_t x
    pw.add_fixed64(17, 1);      // fixed64 r
    pw.add_fixed64(17, 2);
    pw.add_fixed64(17, 3);
    pw.add_string(2, "foobar"); // string s

First you create an empty string which will be used as buffer to assemble the
protobuf-formatted message. The `pbf_writer` object contains a reference to
this string buffer and through it you add data to that buffer piece by piece.


### Handling Scalar Fields

As you could see in the introductory example handling any kind of scalar field
is easy. The type of field doesn't matter and it doesn't matter whether it is
optional, required or repeated. You always call one of the `add_TYPE()` method
on the pbf writer object.

The first parameter of these methods is always the *tag* of the field (the
field number) from the `.proto` file. The second parameter is the value you
want to set. For the `bytes` and `string` types several versions of the add
method are available taking a `std::string` or a `const char*` and a length.

For `enum` types you have to use the numeric value as the symbolic names from
the `.proto` file are not available.


### Handling Repeated Packed Fields

Repeated packed fields can easily be set from a pair of iterators:

    std::string data;
    osmium::util::pbf_writer pw(data);

    std::vector<int> v = { 1, 4, 9, 16, 25, 36 };
    pw.add_packed_int32(1, std::begin(v), std::end(v));


### Handling Sub-Messages

