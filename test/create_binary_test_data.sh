#!/bin/sh
#
#  create_binary_test_data.sh
#
#  Create the serialized test data in protobuf format using the testcase.proto
#  description and the testcase.cpp code.
#

set -e

for dir in t/*; do
    echo "Generating $dir..."
    cd $dir
    if [ -f testcase.proto ]; then
        protoc --cpp_out=. testcase.proto
        $CXX -std=c++11 -o testcase testcase.cpp testcase.pb.cc -lprotobuf-lite -pthreads
        ./testcase
    fi
    cd ../..
done

