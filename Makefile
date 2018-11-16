CXX		?= gcc
CXX_FLAGS 	:= -ansi -pedantic -Werror -Wall -O3 -std=c++17 -fPIC -fext-numeric-literals -ffast-math -flto
TEST_FLAGS 	:= $(CXX_FLAGS) lib/*.so -Iinclude -Iutil -lpthread $(FFT_FLAG)
FFT_FLAG 	?= -lffts

all: build

build: directories
	$(CXX) $(CXX_FLAGS) -shared -o lib/libpitch_detection.so $(FFT_FLAG) src/*.cpp -Iinclude

util: directories
	$(CXX) $(CXX_FLAGS) -shared -o lib/util.so util/*.cpp

test: build util
	$(CXX) $@/*.cpp -o bin/$@ $(TEST_FLAGS) -lgtest

bench: build util
	$(CXX) $@/*.cpp -o bin/$@ $(TEST_FLAGS) -lbenchmark

example: build util bin/sinewave bin/stdin
bin/%: example/%.cpp
	$(CXX) $< lib/*.so -o $@ $(CXX_FLAGS) -o $@ $(FFT_FLAG) -Iinclude -Iutil -lgflags

directories:
	@mkdir -p lib bin

clean:
	-rm -rf lib bin

install: build
	cp include/pitch_detection.h /usr/include
	cp lib/libpitch_detection.so /usr/lib

.PHONY: clean
