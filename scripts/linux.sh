#!/usr/bin/env bash

set -e -u
set -o pipefail

if [[ ${CXX} =~ "clang" ]]; then
    make clean
    CXXFLAGS="--coverage" LDFLAGS="--coverage" make test
    sudo pip install cpp-coveralls
    coveralls -i pbf.hpp --gcov-options '\-lp'
else
    echo "skipping building coverage"
fi

