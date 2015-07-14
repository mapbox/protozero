# first inherit from env
CXX := $(CXX)
CXXFLAGS := $(CXXFLAGS)
LDFLAGS := $(LDFLAGS)

WARNING_FLAGS := -Wall -Wextra -pedantic -Wsign-compare -Wsign-conversion -Wunused-parameter

ifneq ($(findstring clang,$(CXX)),)
    WARNING_FLAGS += -Wno-reserved-id-macro -Weverything -Wno-weak-vtables -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-exit-time-destructors -Wno-switch-enum -Wno-padded
endif

COMMON_FLAGS := -fvisibility-inlines-hidden -std=c++11 $(WARNING_FLAGS)

RELEASE_FLAGS := -O3 -DNDEBUG -march=native
DEBUG_FLAGS := -O0 -g -fno-inline-functions

OS:=$(shell uname -s)
ifeq ($(OS),Darwin)
    CXXFLAGS += -stdlib=libc++
    LDFLAGS += -stdlib=libc++
endif

READER_TEST_CASES := $(wildcard test/t/*/reader_test_cases.cpp)
READER_TEST_CASES_O := $(subst .cpp,.o,$(READER_TEST_CASES))

WRITER_TEST_CASES := $(wildcard test/t/*/writer_test_cases.cpp)
WRITER_TEST_CASES_O := $(subst .cpp,.o,$(WRITER_TEST_CASES))

PROTO_FILES := $(subst writer_test_cases.cpp,testcase.proto,$(WRITER_TEST_CASES))
PROTO_FILES_CC := $(subst .proto,.pb.cc,$(PROTO_FILES))
PROTO_FILES_H := $(subst .proto,.pb.h,$(PROTO_FILES))
PROTO_FILES_O := $(subst .proto,.pb.o,$(PROTO_FILES))

HPP_FILES := include/protozero/exception.hpp \
             include/protozero/varint.hpp \
             include/protozero/pbf_types.hpp \
             include/protozero/pbf_reader.hpp \
             include/protozero/pbf_writer.hpp

CFLAGS_PROTOBUF := $(shell pkg-config protobuf --cflags)
LDFLAGS_PROTOBUF := $(shell pkg-config protobuf --libs-only-L)

all: ./test/reader_tests test/writer_tests

./test/t/%/reader_test_cases.o: test/t/%/reader_test_cases.cpp $(HPP_FILES)
	$(CXX) -c -Iinclude -Itest/include $(CXXFLAGS) $(COMMON_FLAGS) $(DEBUG_FLAGS) $< -o $@

./test/reader_tests.o: test/reader_tests.cpp $(HPP_FILES)
	$(CXX) -c -Iinclude -Itest/include $(CXXFLAGS) $(COMMON_FLAGS) $(DEBUG_FLAGS) $< -o $@

./test/reader_tests: test/reader_tests.o $(READER_TEST_CASES_O)
	$(CXX) $(LDFLAGS) $^ -o $@

./test/t/%/testcase.pb.cc: ./test/t/%/testcase.proto
	protoc --cpp_out=. $^

./test/t/%/testcase.pb.o: ./test/t/%/testcase.pb.cc
	$(CXX) -c -Iinclude -Itest/include $(CXXFLAGS) $(CFLAGS_PROTOBUF) -std=c++11 $(DEBUG_FLAGS) $< -o $@

./test/t/%/writer_test_cases.o: ./test/t/%/writer_test_cases.cpp
	$(CXX) -c -Iinclude -Itest/include $(CXXFLAGS) $(CFLAGS_PROTOBUF) $(COMMON_FLAGS) $(DEBUG_FLAGS) $< -o $@

./test/writer_tests.o: test/writer_tests.cpp $(HPP_FILES) $(PROTO_FILES_CC)
	$(CXX) -c -Iinclude -Itest/include $(CXXFLAGS) $(COMMON_FLAGS) $(DEBUG_FLAGS) $< -o $@

./test/writer_tests: test/writer_tests.o $(PROTO_FILES_O) $(WRITER_TEST_CASES_O)
	$(CXX) $(LDFLAGS) $(LDFLAGS_PROTOBUF) $^ -lprotobuf-lite -pthread -o $@

test: all
	./test/reader_tests
	./test/writer_tests

iwyu: $(HPP_FILES) test/reader_tests.cpp test/writer_tests.cpp
	iwyu -Xiwyu -- -std=c++11 -Iinclude include/protozero/exception.hpp || true
	iwyu -Xiwyu -- -std=c++11 -Iinclude include/protozero/varint.hpp || true
	iwyu -Xiwyu -- -std=c++11 -Iinclude include/protozero/pbf_types.hpp || true
	iwyu -Xiwyu -- -std=c++11 -Iinclude include/protozero/pbf_reader.hpp || true
	iwyu -Xiwyu -- -std=c++11 -Iinclude include/protozero/pbf_writer.hpp || true
	iwyu -Xiwyu -- -std=c++11 -Iinclude -Itest/include test/reader_tests.cpp || true
	iwyu -Xiwyu -- -std=c++11 -Iinclude -Itest/include test/writer_tests.cpp || true

check: $(HPP_FILES) test/reader_tests.cpp test/include/test.hpp test/include/testcase.hpp test/t/*/testcase.cpp $(READER_TEST_CASES)
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
	rm -f ./test/t/*/reader_test_cases.o
	rm -f ./test/t/*/reader_test_cases.gc*
	rm -f ./test/t/*/writer_test_cases.o
	rm -f ./test/t/*/writer_test_cases.gc*
	rm -f ./*.gcov
	rm -fr doc/doxygen_sqlite3.db doc/html coverage

.PHONY: all test iwyu check doc

