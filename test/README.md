
# Tests

Tests are using the [Catch Unit Test Framework](https://github.com/philsquared/Catch).

Call `make test` to compile and run all tests.


## Organization of the test cases

Each test case is in its own directory under the `t` directory. Each directory
contains (some of) the following files:

* `reader_test_cases.cpp`: The C++ source code that runs the read tests.
* `writer_test_cases.cpp`: The C++ source code that runs the write tests.
* `data-*.pbf`: PBF data files used by the tests.
* `testcase.proto`: Protobuf file describing the format of the data files.
* `testcase.cpp`: C++ file for creating the data files.

### Read tests

The `Makefile` automatically finds all the `reader_test_cases.cpp` files and
compiles them. Together with the `reader_tests.cpp` file they make up the
`reader_tests` executable which can be called to execute all the read tests.

### Write tests

The `Makefile` automatically finds all the `writer_test_cases.cpp` files and
compiles them. Together with the `writer_tests.cpp` file they make up the
`writer_tests` executable which can be called to execute all the write tests.

The writer tests need the Google protobuf library to work.


## Creating test data from scratch

Most tests use test data stored in PBF format in their directory. The files
have the suffix `.pbf`. Most of those files have been generated from the
provided `testcase.proto` and `testcase.cpp` files.

Usually you do not have to do this, but if you want to re-generate the PBF
data files, you can do so:

    cd test
    ./create_pbf_test_data.sh

