# first inherit from env
CXX := $(CXX)
CXXFLAGS := $(CXXFLAGS)
LDFLAGS := $(LDFLAGS)
COMMON_FLAGS = -Wall -Wsign-compare -Wsign-conversion -Wshadow -Wunused-parameter -pedantic -fvisibility-inlines-hidden -std=c++11
RELEASE_FLAGS = -O3 -DNDEBUG -march=native -Wpadded
DEBUG_FLAGS = -O0 -g -DDEBUG -fno-inline-functions

OS:=$(shell uname -s)
ifeq ($(OS),Darwin)
    CXXFLAGS += -stdlib=libc++
    LDFLAGS += -stdlib=libc++
endif

UNIT_TESTS=$(wildcard test/t/*/runtest.cpp)

./test/test: test/test.cpp pbf.hpp $(UNIT_TESTS)
	$(CXX) -Itest $(CXXFLAGS) $(COMMON_FLAGS) $(RELEASE_FLAGS) $(LDFLAGS) test/test.cpp $(UNIT_TESTS) -o ./test/test -lz

test: ./test/test
	./test/test

coverage:
	mkdir -p ./out
	$(CXX) -o out/cov-test --coverage test/test.cpp -I./ $(CXXFLAGS) $(COMMON_FLAGS) $(DEBUG_FLAGS) $(LDFLAGS)

clean:
	rm -f ./test/test
	rm -rf ./out
	rm -f test.gc*

.PHONY: test
