#!/usr/bin/env bash

set -e -u
set -o pipefail

if [[ $CXX == "clang++" ]]; then
    make clean
    make coverage
    ./out/cov-test
    cp unit*gc* test/
    sudo pip install cpp-coveralls
    coveralls -i pbf.hpp --gcov-options '\-lp'
fi
