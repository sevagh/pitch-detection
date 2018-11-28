CXX		?= gcc
CXX_FLAGS 	:= -ansi -pedantic -Werror -Wall -O3 -std=c++17 -fPIC -fext-numeric-literals -ffast-math -flto
CXX_PROFILE	:= -ggdb -fno-omit-frame-pointer -fPIC -std=c++17

all: build

build: directories
	$(CXX) $(CXX_FLAGS) -shared -o lib/libpitch_detection.so -lffts src/*.cpp -Iinclude

util: directories
	$(CXX) $(CXX_FLAGS) -shared -o lib/util.so util/*.cpp

test: build util
	$(CXX) $@/*.cpp -o bin/$@ $(CXX_FLAGS) lib/*.so -Iinclude -Iutil -lpthread -lffts -lgtest -lgtest_main

bench: build util
	$(CXX) $@/$@.cpp -o bin/$@ $(CXX_FLAGS) lib/*.so -Iinclude -Iutil -lpthread -lffts -lbenchmark
	$(CXX) $@/mem$@.cpp -o bin/mem$@ $(CXX_FLAGS) lib/*.so -Iinclude -Iutil -lffts -lgflags

profile: CXX_FLAGS=$(CXX_PROFILE)
profile: bench

example: build util bin/sinewave bin/stdin
bin/%: example/%.cpp
	$(CXX) $< lib/*.so -o $@ $(CXX_FLAGS) -o $@ -lffts -Iinclude -Iutil -lgflags

directories:
	@mkdir -p lib bin

fmt:
	find . -regex '.*\.\(cpp\|h\)' -exec clang-format -style=file -i {} \;

clean:
	-rm -rf lib bin

install: build
	cp include/pitch_detection.h /usr/include
	cp lib/libpitch_detection.so /usr/lib

.PHONY: clean
