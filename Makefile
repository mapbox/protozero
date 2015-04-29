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

UNIT_TESTS := $(wildcard test/t/*/runtest.cpp)
UNIT_TESTS_O := $(subst .cpp,.o,$(UNIT_TESTS))

./test/t/%/runtest.o: test/t/%/runtest.cpp pbf.hpp
	$(CXX) -c -Itest $(CXXFLAGS) $(COMMON_FLAGS) $(DEBUG_FLAGS) $< -o $@

./test/test.o: test/test.cpp pbf.hpp
	$(CXX) -c -Itest $(CXXFLAGS) $(COMMON_FLAGS) $(DEBUG_FLAGS) $< -o $@

./test/test: test/test.o $(UNIT_TESTS_O)
	$(CXX) $(LDFLAGS) -lz $^ -o ./test/test

test: ./test/test
	./test/test

coverage:
	mkdir -p ./out
	$(CXX) -o out/cov-test --coverage test/test.cpp -I./ $(CXXFLAGS) $(COMMON_FLAGS) $(DEBUG_FLAGS) $(LDFLAGS)

clean:
	rm -f ./test/test
	rm -f ./test/t/*/testcase.pb.cc
	rm -f ./test/t/*/testcase.pb.h
	rm -f ./test/t/*/testcase
	rm -rf ./out
	rm -f test.gc*

.PHONY: test
