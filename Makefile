# first inherit from env
CXX := $(CXX)
CXXFLAGS := $(CXXFLAGS) -std=c++11
LDFLAGS := $(LDFLAGS)

OS:=$(shell uname -s)
ifeq ($(OS),Darwin)
	CXXFLAGS += -stdlib=libc++
	LDFLAGS += -stdlib=libc++
endif

./test/test: pbf.hpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) test/test.cpp -o ./test/test -lz

test: ./test/test
	./test/test

clean:
	@rm -f ./test/test

.PHONY: test
