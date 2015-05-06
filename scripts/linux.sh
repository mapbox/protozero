#!/usr/bin/env bash

set -e -u
set -o pipefail

if [[ ${CXX} =~ "clang" ]]; then
    make clean
    CXXFLAGS="--coverage" LDFLAGS="--coverage" make test
    pip install --user cpp-coveralls
    ~/.local/bin/coveralls -i pbf.hpp --gcov-options '\-lp'
else
    echo "skipping building coverage"
fi

