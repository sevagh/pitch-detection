CXX		?= gcc
CXX_FLAGS 	:= -ansi -pedantic -Werror -Wall -O3 -std=c++17 -fext-numeric-literals -ffast-math -flto -L$(CURDIR)/../lib -lpitch_detection -lffts -llibnyquist -llibwavpack -llibopus -I$(CURDIR)/../include -Wl,-R$(CURDIR)/../lib

all: wav_analyzer

wav_analyzer:
	$(CXX) ./*.cpp -o $@ $(CXX_FLAGS)

clean:
	-rm wav_analyzer

.PHONY: clean
