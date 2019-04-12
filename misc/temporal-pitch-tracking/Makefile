CXX		?= gcc
CXX_FLAGS 	:= -ansi -pedantic -Werror -Wall -std=c++2a -lffts -lmlpack -fopenmp -larmadillo -llibnyquist -lpitch_detection -lffts -llibwavpack -llibopus -lgflags -ggdb -fno-omit-frame-pointer

all: bin

bin:
	@mkdir -p bin
	$(CXX) -o bin/temporal_pitch_tracker *.cpp $(CXX_FLAGS) 

install: bin
	cp bin/temporal_pitch_tracker /usr/local/bin

fmt:
	find . -regex '.*\.\(cpp\|h\)' -exec clang-format -style=file -i {} \;

clean:
	-rm -rf bin


.PHONY: clean fmt install
