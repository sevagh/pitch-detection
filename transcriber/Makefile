CXX		?= gcc
CXX_FLAGS 	:= -ansi -pedantic -Werror -Wall -O3 -std=c++17 -fext-numeric-literals -ffast-math -flto -lpitch_detection -lffts -llibnyquist -llibwavpack -llibopus

all: transcriber

transcriber:
	$(CXX) ./*.cpp -o $@ $(CXX_FLAGS)

clean:
	-rm transcriber

.PHONY: clean
