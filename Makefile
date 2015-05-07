# first inherit from env
CXX := $(CXX)
CXXFLAGS := $(CXXFLAGS)
LDFLAGS := $(LDFLAGS)

WARNING_FLAGS := -Wall -Wextra -pedantic -Wsign-compare -Wsign-conversion -Wshadow -Wunused-parameter

ifneq ($(findstring clang,$(CXX)),)
    WARNING_FLAGS += -Weverything -Wno-weak-vtables -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-exit-time-destructors -Wno-switch-enum
endif

COMMON_FLAGS := -fvisibility-inlines-hidden -std=c++11 $(WARNING_FLAGS)

RELEASE_FLAGS := -O3 -DNDEBUG -march=native -Wpadded
DEBUG_FLAGS := -O0 -g -DDEBUG -fno-inline-functions

OS:=$(shell uname -s)
ifeq ($(OS),Darwin)
    CXXFLAGS += -stdlib=libc++
    LDFLAGS += -stdlib=libc++
endif

UNIT_TESTS := $(wildcard test/t/*/runtest.cpp)
UNIT_TESTS_O := $(subst .cpp,.o,$(UNIT_TESTS))

all: ./test/run_all_tests

./test/t/%/runtest.o: test/t/%/runtest.cpp pbf.hpp
	$(CXX) -c -I. -Itest/include $(CXXFLAGS) $(COMMON_FLAGS) $(DEBUG_FLAGS) $< -o $@

./test/run_all_tests.o: test/run_all_tests.cpp pbf.hpp
	$(CXX) -c -I. -Itest/include $(CXXFLAGS) $(COMMON_FLAGS) $(DEBUG_FLAGS) $< -o $@

./test/run_all_tests: test/run_all_tests.o $(UNIT_TESTS_O)
	$(CXX) $(LDFLAGS) -lz $^ -o $@

test: ./test/run_all_tests
	./test/run_all_tests

iwyu: pbf.hpp test/run_all_tests.cpp $(UNIT_TESTS)
	iwyu -Xiwyu -- -std=c++11 -I. pbf.hpp || true
	iwyu -Xiwyu -- -std=c++11 -I. -Itest/include test/run_all_tests.cpp || true

check: pbf.hpp test/run_all_tests.cpp test/include/test.hpp test/include/testcase.hpp test/t/*/testcase.cpp $(UNIT_TESTS)
	cppcheck --std=c++11 --enable=all $^

doc: doc/Doxyfile pbf.hpp
	doxygen doc/Doxyfile

clean:
	rm -f ./test/run_all_tests
	rm -f ./test/run_all_tests.o
	rm -f ./test/run_all_tests.gc*
	rm -f ./test/t/*/testcase.pb.cc
	rm -f ./test/t/*/testcase.pb.h
	rm -f ./test/t/*/testcase.o
	rm -f ./test/t/*/testcase
	rm -f ./test/t/*/runtest.o
	rm -f ./test/t/*/runtest.gc*
	rm -fr doc/doxygen_sqlite3.db doc/html

.PHONY: test doc

