CXX		?= gcc
CXX_FLAGS 	:= -ansi -pedantic -Werror -Wall -O3 -std=c++17 -fext-numeric-literals -flto -lffts -L$(CURDIR)/../lib -lpitch_detection -I$(CURDIR)/../include -Wl,-R$(CURDIR)/../lib
CXX_DEBUG_FLAGS := -ansi -pedantic -Werror -Wall -O0 -std=c++17 -fext-numeric-literals -flto -lffts -L$(CURDIR)/../lib -lpitch_detection -I$(CURDIR)/../include -Wl,-R$(CURDIR)/../lib -ggdb

all: test bench

test:
	$(CXX) ./$@*.cpp ./util.cpp -o $@ $(CXX_FLAGS) -lpthread -lgtest -lgtest_main

bench:
	$(CXX) ./$@.cpp ./util.cpp -o $@ $(CXX_FLAGS) -lpthread -lbenchmark

benchmem:
	$(CXX) ./$@.cpp ./util.cpp -o $@ $(CXX_FLAGS) -lgflags

debug: CXX_FLAGS=$(CXX_DEBUG_FLAGS)
debug: test bench benchmem

clean:
	-rm bench benchmem test

.PHONY: clean
