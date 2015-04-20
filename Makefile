# first inherit from env
CXX := $(CXX)
CXXFLAGS := $(CXXFLAGS) -std=c++11
LDFLAGS := $(LDFLAGS)

OS:=$(shell uname -s)
ifeq ($(OS),Darwin)
    CXXFLAGS += -stdlib=libc++
    LDFLAGS += -stdlib=libc++
endif

UNIT_TESTS=$(wildcard test/t/*/runtest.cpp)

./test/test: test/test.cpp pbf.hpp $(UNIT_TESTS)
	$(CXX) -Itest $(CXXFLAGS) $(LDFLAGS) test/test.cpp $(UNIT_TESTS) -o ./test/test -lz

test: ./test/test
	./test/test

clean:
	@rm -f ./test/test

.PHONY: test
