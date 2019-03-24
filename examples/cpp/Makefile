CXX		?= gcc
CXX_FLAGS 	:= -ansi -pedantic -Werror -Wall -O3 -std=c++17 -fext-numeric-literals -ffast-math -flto -lffts -lpitch_detection

all: example

example:
	$(CXX) ./example.cpp -o $@ $(CXX_FLAGS)

clean:
	-rm example

.PHONY: clean
