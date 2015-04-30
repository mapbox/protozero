# first inherit from env
CXX := $(CXX)
CXXFLAGS := $(CXXFLAGS)
LDFLAGS := $(LDFLAGS)

WARNING_FLAGS := -Wall -Wextra -pedantic -Wsign-compare -Wsign-conversion -Wshadow -Wunused-parameter
MORE_WARNING_FLAGS := -Weverything -Wno-weak-vtables -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-exit-time-destructors -Wno-switch-enum
COMMON_FLAGS := -fvisibility-inlines-hidden -std=c++11 $(WARNING_FLAGS) $(MORE_WARNING_FLAGS)

RELEASE_FLAGS := -O3 -DNDEBUG -march=native -Wpadded
DEBUG_FLAGS := -O0 -g -DDEBUG -fno-inline-functions

OS:=$(shell uname -s)
ifeq ($(OS),Darwin)
    CXXFLAGS += -stdlib=libc++
    LDFLAGS += -stdlib=libc++
endif

UNIT_TESTS := $(wildcard test/t/*/runtest.cpp)
UNIT_TESTS_O := $(subst .cpp,.o,$(UNIT_TESTS))

./test/t/%/runtest.o: test/t/%/runtest.cpp pbf.hpp
	$(CXX) -c -I. -Itest/include $(CXXFLAGS) $(COMMON_FLAGS) $(DEBUG_FLAGS) $< -o $@

./test/test.o: test/test.cpp pbf.hpp
	$(CXX) -c -I. -Itest/include $(CXXFLAGS) $(COMMON_FLAGS) $(DEBUG_FLAGS) $< -o $@

./test/test: test/test.o $(UNIT_TESTS_O)
	$(CXX) $(LDFLAGS) -lz $^ -o ./test/test

test: ./test/test
	./test/test

coverage:
	mkdir -p ./out
	$(CXX) -o out/cov-test --coverage test/test.cpp -I./ $(CXXFLAGS) $(COMMON_FLAGS) $(DEBUG_FLAGS) $(LDFLAGS)

clean:
	rm -f ./test/test
	rm -f ./test/test.o
	rm -f ./test/t/*/testcase.pb.cc
	rm -f ./test/t/*/testcase.pb.h
	rm -f ./test/t/*/testcase.o
	rm -f ./test/t/*/testcase
	rm -f ./test/t/*/runtest.o
	rm -rf ./out
	rm -f test.gc*

.PHONY: test
