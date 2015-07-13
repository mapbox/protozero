# first inherit from env
CXX := $(CXX)
CXXFLAGS := $(CXXFLAGS)
LDFLAGS := $(LDFLAGS)

WARNING_FLAGS := -Wall -Wextra -pedantic -Wsign-compare -Wsign-conversion -Wunused-parameter

ifneq ($(findstring clang,$(CXX)),)
    WARNING_FLAGS += -Wno-reserved-id-macro -Weverything -Wno-weak-vtables -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-exit-time-destructors -Wno-switch-enum
endif

COMMON_FLAGS := -fvisibility-inlines-hidden -std=c++11 $(WARNING_FLAGS)

RELEASE_FLAGS := -O3 -DNDEBUG -march=native -Wpadded
DEBUG_FLAGS := -O0 -g -fno-inline-functions

OS:=$(shell uname -s)
ifeq ($(OS),Darwin)
    CXXFLAGS += -stdlib=libc++
    LDFLAGS += -stdlib=libc++
endif

UNIT_TESTS := $(wildcard test/t/*/runtest.cpp)
UNIT_TESTS_O := $(subst .cpp,.o,$(UNIT_TESTS))

WRITE_TESTS := $(wildcard test/t/*/write_tests.cpp)
WRITE_TESTS_O := $(subst .cpp,.o,$(WRITE_TESTS))

PROTO_FILES := $(subst write_tests.cpp,testcase.proto,$(WRITE_TESTS))
PROTO_FILES_CC := $(subst .proto,.pb.cc,$(PROTO_FILES))
PROTO_FILES_H := $(subst .proto,.pb.h,$(PROTO_FILES))
PROTO_FILES_O := $(subst .proto,.pb.o,$(PROTO_FILES))

HPP_FILES := include/protozero/pbf_common.hpp \
             include/protozero/pbf_reader.hpp \
             include/protozero/pbf_writer.hpp

CFLAGS_PROTOBUF := $(shell pkg-config protobuf --cflags)
LDFLAGS_PROTOBUF := $(shell pkg-config protobuf --libs-only-L)

all: ./test/reader_tests test/writer_tests

./test/t/%/runtest.o: test/t/%/runtest.cpp $(HPP_FILES)
	$(CXX) -c -Iinclude -Itest/include $(CXXFLAGS) $(COMMON_FLAGS) $(DEBUG_FLAGS) $< -o $@

./test/reader_tests.o: test/reader_tests.cpp $(HPP_FILES)
	$(CXX) -c -Iinclude -Itest/include $(CXXFLAGS) $(COMMON_FLAGS) $(DEBUG_FLAGS) $< -o $@

./test/reader_tests: test/reader_tests.o $(UNIT_TESTS_O)
	$(CXX) $(LDFLAGS) $^ -o $@

./test/t/%/testcase.pb.cc: ./test/t/%/testcase.proto
	protoc --cpp_out=. $^

./test/t/%/testcase.pb.o: ./test/t/%/testcase.pb.cc
	$(CXX) -c -Iinclude -Itest/include $(CXXFLAGS) $(CFLAGS_PROTOBUF) -std=c++11 $(DEBUG_FLAGS) $< -o $@

./test/t/%/write_tests.o: ./test/t/%/write_tests.cpp
	$(CXX) -c -Iinclude -Itest/include $(CXXFLAGS) $(CFLAGS_PROTOBUF) -std=c++11 $(DEBUG_FLAGS) $< -o $@

./test/writer_tests.o: test/writer_tests.cpp $(HPP_FILES) $(PROTO_FILES_CC)
	$(CXX) -c -Iinclude -Itest/include $(CXXFLAGS) $(COMMON_FLAGS) $(DEBUG_FLAGS) $< -o $@

./test/writer_tests: test/writer_tests.o $(PROTO_FILES_O) $(WRITE_TESTS_O)
	$(CXX) $(LDFLAGS) $(LDFLAGS_PROTOBUF) $^ -lprotobuf-lite -pthread -o $@

test: all
	./test/reader_tests
	./test/writer_tests

iwyu: $(HPP_FILES) test/reader_tests.cpp $(UNIT_TESTS)
	iwyu -Xiwyu -- -std=c++11 -Iinclude protozero/pbf_common.hpp || true
	iwyu -Xiwyu -- -std=c++11 -Iinclude protozero/pbf_reader.hpp || true
	iwyu -Xiwyu -- -std=c++11 -Iinclude protozero/pbf_writer.hpp || true
	iwyu -Xiwyu -- -std=c++11 -Iinclude -Itest/include test/reader_tests.cpp || true

check: $(HPP_FILES) test/reader_tests.cpp test/include/test.hpp test/include/testcase.hpp test/t/*/testcase.cpp $(UNIT_TESTS)
	cppcheck -Uassert --std=c++11 --enable=all --suppress=incorrectStringBooleanError $^

doc: doc/Doxyfile README.md tutorial.md $(HPP_FILES)
	doxygen doc/Doxyfile

clean:
	rm -f ./test/reader_tests
	rm -f ./test/reader_tests.o
	rm -f ./test/reader_tests.gc*
	rm -f ./test/writer_tests
	rm -f ./test/writer_tests.o
	rm -f ./test/writer_tests.gc*
	rm -f ./test/t/*/testcase.pb.cc
	rm -f ./test/t/*/testcase.pb.h
	rm -f ./test/t/*/testcase.pb.o
	rm -f ./test/t/*/testcase.o
	rm -f ./test/t/*/testcase
	rm -f ./test/t/*/runtest.o
	rm -f ./test/t/*/runtest.gc*
	rm -f ./test/t/*/write_tests.o
	rm -f ./test/t/*/write_tests.gc*
	rm -f ./*.gcov
	rm -fr doc/doxygen_sqlite3.db doc/html coverage

.PHONY: all test iwyu check doc

